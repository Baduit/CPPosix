#include <cassert>
#include <iostream>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include <list>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Fd.hpp"
#include "FileFd.hpp"
#include "SocketFd.hpp"
#include "PipeFd.hpp"
#include "FdFunctions.hpp"
#include "Dir.hpp"

using namespace Cpposix;
using namespace std::string_literals;
using namespace std::string_view_literals;

void test_write()
{
	Fd wrong_fd(-4);
	assert(!wrong_fd);
	assert(!wrong_fd.write(5));

	Fd fd(1);
	assert(fd);
	assert(fd.write(static_cast<uint8_t>('I')).getOr(0) == sizeof(uint8_t));

	std::string str = "\nContainer";
	assert(fd.write(str).getOr(0) == str.size());

	assert(fd.write("\n", 1).getOr(0) == 1);

	assert((fd << 'O'));
	assert((fd << "perator <<\n"s));
}

void test_read()
{
	FileFd fd("Makefile", O_RDONLY);

	assert(fd);

	int32_t i;
	assert(fd.read(i).getOr(0) == sizeof(int32_t));

	std::vector<uint8_t> v(10);
	assert(fd.read(v).getOr(0) == 10);
	
	for (uint8_t c: v)
		std::cout << c;
	std::cout << std::endl;
}

void test_readExact()
{
/* 	FileFd fd("Makefile", O_RDONLY);

	assert(fd);
	assert(fd.readExact<uint32_t>());
	assert(fd.readExact<std::string>(50)); */
}

void test_pipe()
{
	Pipe p;
	assert(p);

	assert(p.write(static_cast<std::byte>('A')));
	char c;
	assert(p.read(c) == 1);
	assert(c == 'A');

	auto input = "salut les amis"sv;
	auto res = p.write(input);
	assert(res);
	assert(*res == input.size());

	std::string output;
	output.resize(input.size());
	assert(p.read(output).getOr(0) == input.size());
	assert(input == output);
}

void test_dup()
{
	FileFd dev_null("/dev/null");
	Fd standard_output(1);
	assert(dev_null);
	{
		standard_output.write(std::string("You should see \"Success\" in the next line\n"));
		RedirectFd redirection(standard_output, dev_null);
		standard_output.write(std::string("ERROR\n"));
	}
	standard_output.write(std::string("Success\n"));
}

void test_select()
{
	{
		FileFd fd("Makefile", O_RDONLY);
		FdEvent fd_event { { fd }, {}, {} };

		auto result = fdSelect(fd_event);
		assert(result);
		assert(result->read[0].get().getFdAsInt() == fd.getFdAsInt());
	}
	{
		FileFd fd(0);
		FdEvent fd_event { { fd }, {}, {} };

		std::cout << "Wait 1s begin, it will be shorter you write in the command line" << std::endl;
		auto result = fdSelect(fd_event, std::chrono::seconds(1));
		std::cout << "Wait 1s end" << std::endl;
	}
}

int main()
{
	test_write();
	test_read();
	test_readExact();
	test_pipe();
	test_dup();
	test_select();
}