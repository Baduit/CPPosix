#pragma once

#include <variant>

namespace Cpposix
{

class Error
{
	public:
		Error() = default;

		int get_errno_value() const { return _errno; }
	private:
		int _errno = 0;
};

template <typename T>
class Expected
{
	public:
		Expected(const T& t): _variant(t) {}
		Expected(Error e): _variant(e) {}


		T& operator*() { return get(); }
		const T& operator*() const { return get(); }

		T* operator->() { return &std::get<1>(_variant); }
		const T* operator->() const { return &std::get<1>(_variant); }

		T& get() { return std::get<1>(_variant); }
		const T& get() const { return std::get<1>(_variant); }

		T get_or(const T& t)
		{
			if (_variant.index() == 1)
				return get();
			else
				return t;
		}
		T get_or(const T& t) const
		{
			if (_variant.index() == 1)
				return get();
			else
				return t;
		}

		Error get_error() const { return std::get<0>(_variant); }

		operator bool() { return _variant.index(); }
		operator bool() const { return _variant.index(); }

	private:
		std::variant<Error, T> _variant;
};

}