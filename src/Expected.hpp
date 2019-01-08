#pragma once

#include <variant>

namespace Cpposix
{

class Error
{
	public:

		int get_errno_value() const { return _errno; }
	private:
		int _errno = 0;
};

template <typename T>
class Expected
{
	public:
		Expected(const T& t);
		//Expected(T t); pour les smalls types plus tard
		Expected(Error);

		// operator * const et non const
		// operator -> const et non const

		// get const et non const
		// get_or const et non const

		// get_error const 

		// operator bool() const

	private:
		std::variant<Error, T> _variant;
};

}