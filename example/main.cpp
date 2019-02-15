#include <iostream>
#include <string>
#include <string_view>
#include <exception>
#include <cassert>
#include <thread>
#include <chrono>

#include "FileFd.hpp"
#include "PipeFd.hpp"
#include "Dir.hpp"
#include "Filesystem.hpp"
#include "Process.hpp"

using namespace std::literals;

int main(int argc, char **argv)
{
	// On vérifie qu'on a bien un argument
	if (argc <= 1)
		throw std::runtime_error("Please send the path of the config file as the first argument");

	// Ouvrir un fichier
	Cpposix::FileFd	conf_file(argv[1]);
	if (!conf_file)
		throw std::runtime_error("Conf file opening error.");

	// Appeler la fonction stat pour récupérer la taille du fichier
	auto stat_result = conf_file.stat();
	if (!stat_result)
		throw std::runtime_error("Stat error.");
	std::size_t file_size = static_cast<std::size_t>(stat_result->st_size);

	// Lire le fichier en une fois pour avoir un seul appel systeme
	auto conf_file_content = conf_file.readExact<std::string>(file_size);
	if (!conf_file_content)
		throw std::runtime_error("Error while reading the conf file");


	std::vector<std::string> process_to_execute;
	// Parcourir un dossier
	for (const auto& filename: Cpposix::DirFd(*conf_file_content))
	{
		// Utiliser stat pour filtrer seulement des fichiers "normaux" et via le nom du fichier
		auto file_stat_result = Cpposix::stat(filename);
		if (file_stat_result && S_ISREG(file_stat_result->st_mode) && filename.find(".test") != std::string::npos)
			process_to_execute.push_back(filename);
	}

	
	for (const auto& process_name: process_to_execute)
	{
		// Callback qui sera exécuté dans le processus fils
		auto cb =
			[&](Cpposix::Pipe& com)
			{
				std::cout << "Waiting main process to start execution." << std::endl;
				com.read<uint8_t>();
				std::cout << "****************************************" << std::endl;
				std::cout << "Test: " << process_name << " is begining" << std::endl;
				std::cout << "****************************************" << std::endl;

				// On lance le processus
				Cpposix::exec("./" + process_name, {});
				// Exec ne renvoie jamais en cas de succès
				std::cout << "Execution of a process failed." << std::endl;
			}
		;

		// Création du l'interface de communication
		Cpposix::Pipe com;

		// Création du processus
		Cpposix::ChildProcess child_process(cb, com);

		// On attend un peu avant de lancer le processus (pas obligatoire mais c'est pour montrer que la communication marche bien)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		// On écrit tu processus fils pour qu'il se lance
		std::cout << "Writing to child process to start execution." << std::endl;
		com.write(static_cast<uint8_t>(1));

		// On attend la fin du processus fils et on récupère son code de retour, si l'attente échoue on dit que le code vaut -1 (au lieu de 0 qui est attendu normalement)
		int result = child_process.wait().getOr(-1);

		std::cout << "****************************************" << std::endl;
		std::cout << "Test: " << process_name << " ended with code: "<< result << " (0 is success)" << std::endl;
		std::cout << "****************************************" << std::endl << std::endl << std::endl;

		// On attend un peu avant de lancer un autre test
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}