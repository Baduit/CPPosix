#include <cassert>
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <thread>
#include <chrono>

#include "Process.hpp"

using namespace Cpposix;

int main()
{
	{
		auto cb =
			[]()
			{
				std::cout << "It is working! "<< std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				return 5;
			};

		ChildProcess p(cb);
		auto result = p.wait();
		assert(result);
		assert(result.getOr(77) == 5);
	}

	{
		auto cb =
			[]()
			{
				std::cout << "It is working! "<< std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			};

		ChildProcess p(cb);
		auto result = p.wait();
		assert(result);
		assert(result.getOr(77) == 0);
	}

	{
		auto cb =
			[]()
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
				std::cout << "It is not working! :'("<< std::endl;
			};

		ChildProcess p(cb);
		assert(p.kill());
		assert(p.wait());
	}
}