#pragma once

#include <variant>
#include <cerrno>
#include <cstring>
#include <string>
#include <string_view>
#include <mutex>
#include <exception>

namespace Cpposix
{

class CpposixException
{
	public:
		explicit CpposixException(std::string_view msg): _msg(msg) {}

		const char* what() const { return _msg.c_str(); }

	private:
		std::string _msg;
};

std::string_view strerror(int errnum)
{
	static std::mutex mutex;
	std::lock_guard lock(mutex);
	return std::string_view(std::strerror(errnum));
}

class Error
{
	public:
		Error(): _errno(errno) {}
		Error(int errnum): _errno(errnum) {}

		int getErrnoValue() const { return _errno; }

		std::string getErrorMessage() const
		{
			return std::string(strerror(_errno));
		};

		std::string_view getErrorMessageView() const
		{
			return strerror(_errno);
		}

	private:
		int _errno;
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