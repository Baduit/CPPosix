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
	check_string_view_null_terminated(name);
	const char *cstr = getenv(name.data());
	if (!cstr)
		return Error();
	return std::string(cstr);
}

Expected<std::string_view>	getEnvView(std::string_view name)
{
	check_string_view_null_terminated(name);
	const char *cstr = getenv(name.data());
	if (!cstr)
		return Error();
	return std::string_view(cstr);
}

Expected<Void>	setEnv(std::string_view name, std::string_view value, bool overwrite = true)
{
	check_string_view_null_terminated(name);
	int overwrite_int = (overwrite) ? 1: 0;
	if (setenv(name.data(), value.data(), overwrite_int) == -1)
		return Error();
	return Void();
}

// no putenv because of differents implementation in the glibc, with possible memory leak and bad handling of ownership
// always use setEnv


Expected<Void>	unsetEnv(std::string_view name)
{
	check_string_view_null_terminated(name);
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

struct EnvValue
{
	EnvValue& operator=(std::string_view str)
	{
		if (setEnv(key, str))
		{
			value = str;
			valid = true;
		}
		return *this;
	}

	operator std::string() const { return value; }
	operator bool() const { return valid; }

	bool operator==(std::string_view str) { return value == str; }
	bool operator!=(std::string_view str) { return value != str; }

	std::string value;
	std::string key;
	bool valid = true;
};

// overhead here, but make something practical to use and without overhead would take some time i don't have
class Env
{
	public:
		Env(): _env(environ)
		{
			// some stuff later
			// man 7 environ
		}

		EnvValue	operator[](std::string_view key)
		{
			auto value = getEnv(key);
			if (value)
			{
				return EnvValue { *value, std::string(key), true };
			}
			else
			{
				return EnvValue { "", std::string(key), false };
			}
			
		}

		char** _env;
};

}