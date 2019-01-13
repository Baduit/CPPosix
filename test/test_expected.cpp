#include <cassert>

#include "Expected.hpp"

using namespace Cpposix;

void test_error()
{
	Error e;
	assert(e.get_errno_value() == 0);
}

void test_expected()
{
	Expected<int> eint = 5;
	assert(eint);
	assert(eint.get() == 5);
	assert(eint.get_or(10) == 5);
	assert(*eint == 5);

	Expected<int> eerr = Error {};
	assert(eerr.get_or(10) == 10);
	assert(eerr.get_error().get_errno_value() == Error().get_errno_value());
	assert(!eerr);
}

int main()
{
	test_error();
	test_expected();
}