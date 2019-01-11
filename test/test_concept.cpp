#include <cassert>
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <list>

#include "Concept.hpp"

using namespace Cpposix;
using namespace std::string_literals;


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
	WRITABLE,
	WRITABLE_CONTAINER,
	NONE
};

struct NoneCompile
{
	using value_type = int;

	std::size_t size() const { return 0; }
	std::size_t data() const { return 0; }
};

template <typename Writable, typename = IsWritable<Writable>>
OverloadUsed my_function(Writable writable)
{
	return OverloadUsed::WRITABLE;
}

template <typename WritableContainer, typename = IsWritableContainer<WritableContainer>>
OverloadUsed my_other_function(WritableContainer writableContainer)
{
	return OverloadUsed::WRITABLE_CONTAINER;
}

void testIsWritable()
{
	assert(Cpposix::isWritable<int>());
	assert(Cpposix::isWritable<packed>());
	assert(!Cpposix::isWritable<double>());
	assert(!Cpposix::isWritable<not_packed>());
	assert(!Cpposix::isWritable<void*>());
	assert(!Cpposix::isWritable<int*>());
	assert(!Cpposix::isWritable<std::string>());

	assert(my_function(7) == OverloadUsed::WRITABLE);
}

void testIsWritableContainer()
{
	assert(Cpposix::isWritableContainer<std::string>());
	assert(Cpposix::isWritableContainer<std::vector<int>>());
	assert(!Cpposix::isWritableContainer<std::vector<double>>());

	// theses lines does not compile on purpose
	//assert(!Cpposix::isWritableContainer<int>());
	//assert(!Cpposix::isWritableContainer<packed>());
	//assert(!Cpposix::isWritableContainer<std::list<int>>());
	//assert(!Cpposix::isWritableContainer<int*>());

	assert(my_other_function("hey"s) == OverloadUsed::WRITABLE_CONTAINER);
}


template <typename T>
OverloadUsed choose_good_function(const T& t)
{
	if constexpr (isWritable<T>())
		return my_function(t);
	else if constexpr (isWritableContainer<T>())
		return my_other_function(t);
	else
		return OverloadUsed::NONE;
}

void test_choose_function()
{
	assert(choose_good_function(5) == OverloadUsed::WRITABLE);
	assert(choose_good_function(""s) == OverloadUsed::WRITABLE_CONTAINER);
	// does not compile on purpose
	//assert(choose_good_function(std::list {50, 25}) == OverloadUsed::NONE);
	assert(choose_good_function(NoneCompile()) == OverloadUsed::NONE);
}

int main()
{
	testIsWritable();
	testIsWritableContainer();
	test_choose_function();
}