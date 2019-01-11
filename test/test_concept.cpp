#include <cassert>
#include <iostream>
#include <cstdint>

#include "Concept.hpp"

using namespace Cpposix;


#pragma pack(1)
struct packed
{
	int a;
	int b;
	uint8_t c;
};
#pragma pack()

struct not_packed
{
	int a;
	int b;
	uint8_t c;
};

enum class OverloadUsed
{
	WRITTABLE,
	WRITTABLE_SMART_POINTER,
	WRITTABLE_CONTAINER,
	NONE
};

template <typename Writtable, typename = IsWrittable<Writtable>>
OverloadUsed my_function(Writtable writtable)
{
	return OverloadUsed::WRITTABLE;
}

/* template <typename T, typename = IsUnwrittable<T>>
OverloadUsed my_function(T t)
{
	return OverloadUsed::NONE;
} */

void testIsWrittable()
{
	assert(Cpposix::isWrittable<int>());
	assert(!Cpposix::isWrittable<double>());
	assert(Cpposix::isWrittable<packed>());
	assert(!Cpposix::isWrittable<not_packed>());
	assert(!Cpposix::isWrittable<void*>());

	assert(Cpposix::isWrittable(5));
	assert(!Cpposix::isWrittable(5.5));
	assert(Cpposix::isWrittable(packed()));
	assert(!Cpposix::isWrittable(not_packed()));
	assert(!Cpposix::isWrittable(testIsWrittable));

	assert(my_function(7) == OverloadUsed::WRITTABLE);
}

int main()
{
	testIsWrittable();
}