#pragma once

#include <type_traits>
#include <utility>

namespace Cpposix
{

namespace Details
{

template<typename T, typename _ = void>
struct IsContainerStruct : std::false_type {};

template<typename... Ts>
struct IsContainerStruct_helper {};

template<typename T>
struct IsContainerStruct
<
	T,
	std::conditional_t
	<
		false,
		IsContainerStruct_helper
		<
			typename T::value_type,
			decltype(std::declval<T>().size()),
			decltype(std::declval<T>().data())
		>,
		void
	>
> : public std::true_type {};


template <typename T>
inline constexpr bool isContainer()
{
    if constexpr (std::is_base_of_v<std::true_type, IsContainerStruct<T>>)
        return true;
    else
        return false;
}

}

}