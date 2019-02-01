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

using namespace Cpposix;
using namespace std::string_literals;

void test_write()
{
	Fd wrong_fd(-4);
	assert(!wrong_fd);
	assert(!wrong_fd.write(5));

	Fd fd(1);
	assert(fd);
	assert(fd.write(static_cast<uint8_t>('I')).get_or(0) == sizeof(uint8_t));

	std::string str = "\nContainer";
	assert(fd.write(str).get_or(0) == str.size());
	assert(fd.write(str, 50).get_or(0) == 0);

	std::string str2 = "\nContainerblabla";
	assert(fd.write(str2, str.size()).get_or(0) == str.size());

	assert(fd.write("\n", 1).get_or(0) == 1);

	assert((fd << "Operator <<\n"s));
}

void test_readIn()
{
	FileFd fd("Makefile", O_RDONLY);

	assert(fd);

	int32_t i;
	assert(fd.readIn(i).get_or(0) == sizeof(int32_t));

	std::vector<uint8_t> v(10);
	assert(fd.readIn(v).get_or(0) == 10);
	assert(fd.readIn(v, 5).get_or(0) == 5);
	assert(fd.readIn(v.data(), 5).get_or(0) == 5);
	assert((fd >> v));
	assert(!fd.readIn(v, 25));
}

void test_read()
{
	FileFd fd("Makefile", O_RDONLY);

	assert(fd);
	assert(fd.read<uint32_t>());
	assert(fd.read<std::string>(50));
}

void test_readExact()
{
	FileFd fd("Makefile", O_RDONLY);

	assert(fd);
	assert(fd.readExact<uint32_t>());
	assert(fd.readExact<std::string>(50));
}

int main()
{
	test_write();
	test_readIn();
	test_read();
	test_readExact();
}