#pragma once

#include <type_traits>

namespace Cpposix
{
/*
	HELPER FUNCTIONS
*/
	
template <typename T>
inline constexpr bool isWrittable()
{
	return (std::has_unique_object_representations<T>::value) && (!std::is_pointer<T>::value);
}

template <typename T>
inline constexpr bool isWrittable(const T& t) { return isWrittable<T>(); }


template <typename T>
inline constexpr bool isSmartPointer();

template <typename T>
inline constexpr bool isWrittabeSmartPointer();


/*
CONCEPTS

Writtable => unique_representatoin && not a pointer
	example:
		template<typename Writtable>
		void test(Writtable writtable);

SmartPointer
WrittableSmartPointer

ContiguousContainer pas sur que ça soit utile
WrittableContainer pas sur que ça soit utile


ForkCallback

*/

template <typename T>
using IsWrittable = std::enable_if_t<isWrittable<T>()>;


// used for better error message with static_assert
template <typename T>
using IsUnwrittable = std::enable_if_t<!isWrittable<T>()/*ajouter ici les autres négations de concepts plus tard*/>;

}