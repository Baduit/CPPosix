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
	/*
	** Lire un fichier de configuration
	*/
	if (argc <= 1)
		throw std::runtime_error("Please send the path of the config file as the first argument");

	Cpposix::FileFd	conf_file(argv[1]);
	if (!conf_file)
		throw std::runtime_error("Conf file opening error.");

	auto stat_result = conf_file.stat();
	if (!stat_result)
		throw std::runtime_error("Stat error.");

	std::size_t file_size = static_cast<std::size_t>(stat_result->st_size);
	auto conf_file_content = conf_file.readExact<std::string>(file_size);
	if (!conf_file_content)
		throw std::runtime_error("Error while reading the conf file");

	/*
	** Parcourir un dossier
	*/
	std::vector<std::string> process_to_execute;

	for (const auto& filename: Cpposix::DirFd(*conf_file_content))
	{
		auto file_stat_result = Cpposix::stat(filename);
		if (file_stat_result && S_ISREG(file_stat_result->st_mode) && filename.find(".test") != std::string::npos)
			process_to_execute.push_back(filename);
	}

	/*
	** Lancer un sous processus, envoyer un message 
	*/
	
	for (const auto& process_name: process_to_execute)
	{
		auto cb =
			[&](Cpposix::Pipe& com)
			{
				std::cout << "Waiting main process to start execution." << std::endl;
				com.read<uint8_t>();
				std::cout << "****************************************" << std::endl;
				std::cout << "Test: " << process_name << " is begining" << std::endl;
				std::cout << "****************************************" << std::endl;
				Cpposix::exec("./" + process_name, {});
				std::cout << "Execution of a process failed." << std::endl;
			}
		;

		Cpposix::Pipe com;
		Cpposix::ChildProcess child_process(cb, com);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		std::cout << "Writing to child process to start execution." << std::endl;
		com.write(static_cast<uint8_t>(1));
		int result = child_process.wait().getOr(-1);
		std::cout << "****************************************" << std::endl;
		std::cout << "Test: " << process_name << " ended with code: "<< result << " (0 is success)" << std::endl;
		std::cout << "****************************************" << std::endl << std::endl << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}