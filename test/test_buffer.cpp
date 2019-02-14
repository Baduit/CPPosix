#include <cassert>
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <list>

#include "Buffer.hpp"

using namespace Cpposix;

void dynamic_buffer()
{
	DynamicBuffer buff;

	buff << static_cast<uint8_t>(10);
	
	uint8_t a = 0;
	buff >> a;
	assert(a == 10);
	assert(buff.empty());

	buff << static_cast<uint8_t>(11);
	buff << static_cast<uint8_t>(12);

	buff >> a;
	assert(a == 11);
	assert(buff.size() == 1);
	
	buff >> a;
	assert(a == 12);
	assert(buff.empty());
}

void static_buffer()
{
	
}

int main()
{
	dynamic_buffer();
	static_buffer();
}