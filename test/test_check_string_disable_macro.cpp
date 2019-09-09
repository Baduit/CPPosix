#include <cassert>
#include <iostream>
#include <unistd.h>

#define CPPOSIX_MACRO_DISABLE_CHECK_STRING_VIEW_NULL_TERMINATED
#include "Expected.hpp"

using namespace Cpposix;

void test_check_string_disable_macro()
{
	{
		bool catched = false;
		try
		{
			std::string_view str_view = "123456789";
			std::string_view substr_view = str_view.substr(0, 2);
			check_string_view_null_terminated(substr_view);
		}
		catch (const CpposixException& e)
		{
			catched = true;
		}
		assert(!catched);
	}
}

int main()
{
	test_check_string_disable_macro();
}