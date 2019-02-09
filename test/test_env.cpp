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
}