#include <cassert>
#include <iostream>
#include <cstdint>
#include <string>
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

void test_write()
{
	/* Fd wrong_fd(-4);
	assert(!wrong_fd);
	assert(!wrong_fd.write(5));

	Fd fd(1);
	assert(fd);
	assert(fd.write(static_cast<uint8_t>('I')).getOr(0) == sizeof(uint8_t));

	std::string str = "\nContainer";
	assert(fd.write(str).getOr(0) == str.size());
	assert(fd.write(str, 50).getOr(0) == 0);

	std::string str2 = "\nContainerblabla";
	assert(fd.write(str2, str.size()).getOr(0) == str.size());

	assert(fd.write("\n", 1).getOr(0) == 1);

	assert((fd << "Operator <<\n"s)); */
}

void test_readIn()
{
/* 	FileFd fd("Makefile", O_RDONLY);

	assert(fd);

	int32_t i;
	assert(fd.readIn(i).getOr(0) == sizeof(int32_t));

	std::vector<uint8_t> v(10);
	assert(fd.readIn(v).getOr(0) == 10);
	assert(fd.readIn(v, 5).getOr(0) == 5);
	assert(fd.readIn(v.data(), 5).getOr(0) == 5);
	assert((fd >> v));
	assert(!fd.readIn(v, 25)); */
}

void test_read()
{
/* 	FileFd fd("Makefile", O_RDONLY);

	assert(fd);
	assert(fd.read<uint32_t>());
	assert(fd.read<std::string>(50)); */
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
/* 	Pipe p;
	assert(p);

	assert(p.write(static_cast<std::byte>('A')) == 1);
	char c;
	assert(p.readIn(c) == 1);
	assert(c == 'A');

	assert(p.write(static_cast<std::byte>('C')) == 1);
	assert(p.read<uint8_t>().getOr('B') == 'C'); */
}

void test_dup()
{
/* 	FileFd dev_null("/dev/null");
	Fd standard_output(1);
	assert(dev_null);
	{
		standard_output.write(std::string("You should see \"Success\" in the next line\n"));
		RedirectFd redirection(standard_output, dev_null);
		standard_output.write(std::string("ERROR\n"));
	}
	standard_output.write(std::string("Success\n")); */
}

void test_select()
{
/* 	{
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
	} */
}

int main()
{
	test_write();
	test_readIn();
	test_read();
	test_readExact();
	test_pipe();
	test_dup();
	test_select();
}