#include <cassert>

#include "Expected.hpp"

using namespace Cpposix;

int main()
{
	Error e;
	assert(e.get_errno_value() == 0);
}