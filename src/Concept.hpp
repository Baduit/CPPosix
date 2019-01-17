#pragma once

#include <type_traits>
#include <utility>

namespace Cpposix
{

namespace Constexpr
{
	constexpr bool True = std::integral_constant<bool, true>();
	constexpr bool False = std::integral_constant<bool, false>();
}

template <typename T>
inline constexpr bool isWritable()
{
	return
		std::has_unique_object_representations<T>::value &&
		!std::is_pointer<T>::value
	;
}

template <typename T>
inline constexpr bool isWritableContainer()
{
	return
		isWritable<typename T::value_type>() &&
		std::is_unsigned<decltype(std::declval<T>().size())>::value &&
		(
			std::is_same<typename T::value_type*, decltype(std::declval<T>().data())>::value ||
			std::is_same<const typename T::value_type*, decltype(std::declval<T>().data())>::value
		)
	;
}

template <typename T>
inline constexpr bool isReadableContainer()
{
	return
		isWritable<typename T::value_type>() &&
		std::is_unsigned<decltype(std::declval<T>().size())>::value &&
		std::is_same<typename T::value_type*, decltype(std::declval<T>().data())>::value
	;
}

template <typename T>
using IsWritable = typename std::enable_if_t<isWritable<T>()>;

template <typename T>
using IsReadable = IsWritable<T>;

template <typename T>
using IsWritableContainer = typename std::enable_if_t<isWritableContainer<T>()>;

template <typename T>
using IsReadableContainer = typename std::enable_if_t<isReadableContainer<T>()>;


}