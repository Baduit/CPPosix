#include <cassert>
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <list>

#include "Env.hpp"

using namespace Cpposix;

int main()
{
	{
		assert(setEnv("yolol", "plop"));
		auto yolol_env = getEnv("yolol");
		assert(yolol_env);
		assert(yolol_env.get() == "plop");
	}

	{
		assert(setEnv("yolol", "salut"));
		auto yolol_env = getEnv("yolol");
		assert(yolol_env);
		assert(yolol_env.get() == "salut");
	}

	{
		assert(setEnv("yolol", "kkkk", false));
		auto yolol_env = getEnv("yolol");
		assert(yolol_env);
		assert(yolol_env.get() == "salut");
	}

	// this test can fail if someone already named a var env zefzefzefzefzefzef bur seriouslu who would do that???
	{
		assert(setEnv("zefzefzefzefzefzef", "kkkk", false));
		auto yolol_env = getEnv("zefzefzefzefzefzef");
		assert(yolol_env);
		assert(yolol_env.get() == "kkkk");
	}

	{
		Env env;
		assert(!env["ezzezegzeg"]);
		assert(env["yolol"] == "salut");

		env["ttttt"] = "888";
		assert(env["ttttt"] == "888");

		env["yolol"] = "not_salut_anymore";
		assert(env["yolol"] == "not_salut_anymore");
	}
}