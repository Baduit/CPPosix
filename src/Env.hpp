#pragma once

#include <cstdlib>
#include <string_view>
#include <string>
#include <unistd.h>
#include "Expected.hpp"
#include "Concept.hpp"

namespace Cpposix
{

Expected<std::string>	getEnv(std::string_view name)
{
	if (!name.back())
		throw CpposixException("The string argument is not null terminated");
	const char *cstr = getenv(name.data());
	if (!cstr)
		return Error();
	return std::string(cstr);
}

Expected<std::string_view>	getEnvView(std::string_view name)
{
	if (!name.back())
		throw CpposixException("The string argument is not null terminated");
	const char *cstr = getenv(name.data());
	if (!cstr)
		return Error();
	return std::string_view(cstr);
}

Expected<Void>	setEnv(std::string_view name, std::string_view value, bool overwrite = true)
{
	if (!name.back() || !value.back())
		throw CpposixException("The string argument is not null terminated");
	int overwrite_int = (overwrite) ? 1: 0;
	if (setenv(name.data(), value.data(), overwrite_int) == -1)
		return Error();
	return Void();
}

// no putenv because of differents implementation in the glibc, with possible memory leak and bad handling of ownership
// always use setEnv


Expected<Void>	unsetEnv(std::string_view name)
{
	if (!name.back())
		throw CpposixException("The string argument is not null terminated");
	if (unsetenv(name.data()) == -1)
		return Error();
	return Void();
}

Expected<Void>	clearEnv()
{
	if (clearenv() == -1)
		return Error();
	return Void();
}

class Env
{
	public:
		Env()
		{
			// some stuff later
		}

		//operator [] -> the special struct optional ref asignable

		// iterators
		// create my own iterators
	private:

};

}