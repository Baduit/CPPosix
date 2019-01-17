#include <cassert>
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <list>

#include "Fd.hpp"

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

	std::string str2 = "\nContainerblabla";
	assert(fd.write(str2, str.size()).get_or(0) == str.size());

	assert(fd.write("\n", 1).get_or(0) == 1);

	assert((fd << "Operator <<\n"s));
}

int main()
{
	test_write();
}