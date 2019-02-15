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

	{
		auto cb =
			[](Pipe& com)
			{
				auto a = com.read<uint8_t>();
				assert(a);
				assert(*a == 5);
				auto nb = com.write(static_cast<uint16_t>(15));
				assert(nb);
				assert(*nb == sizeof(uint16_t));
			};

		Pipe com;
		ChildProcess p(cb, com);
		com.write(static_cast<uint8_t>(5));
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		auto b = com.read<uint16_t>();
		assert(b);
		assert(*b == 15);
		p.wait();
	}
}