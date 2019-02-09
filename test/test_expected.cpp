#include <cassert>
#include <iostream>
#include <unistd.h>

#include "Expected.hpp"

using namespace Cpposix;

void test_error()
{
	Error e;
	assert(e.getErrnoValue() == 0);
	
	write(-1, "", 0);
	Error ee;
	assert(ee.getErrnoValue() == EBADF);
	assert(ee.getErrorMessage() != "");
	assert(ee.getErrorMessage() == ee.getErrorMessageView());
}

void test_expected()
{
	Expected<int> eint = 5;
	assert(eint);
	assert(eint.get() == 5);
	assert(eint.getOr(10) == 5);
	assert(*eint == 5);

	Expected<int> eerr = Error {};
	assert(eerr.getOr(10) == 10);
	assert(eerr.getError().getErrnoValue() == Error().getErrnoValue());
	assert(!eerr);
}

void test_exception()
{
	bool catched = false;
	try
	{
		throw CpposixException("Salut");
	}
	catch (const CpposixException& e)
	{
		assert(std::string_view(e.what()) == "Salut");
		catched = true;
	}
	assert(catched);
}

int main()
{
	test_error();
	test_expected();
	test_exception();
}