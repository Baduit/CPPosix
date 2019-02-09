#include <cassert>
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <list>

#include "Filesystem.hpp"
#include "Dir.hpp"
#include "FileFd.hpp"

using namespace Cpposix;
using namespace std::literals;

int main()
{
	assert(mkdir("test"sv));

	assert(FileFd::create("test/test.txt"sv));
	assert(rename("test/test.txt"sv, "test/renamed.txt"sv));

	auto stat_info = stat("test"sv);
	assert(stat_info);
	assert(S_ISDIR(stat_info->st_mode));

	assert(chmod("test/renamed.txt"sv, S_IRWXU));

	auto stat_vfs_info = statVfs("test/renamed.txt"sv);
	assert(stat_vfs_info);

	{
		bool file_found = false;
		for (const auto& filename: DirFd("test"sv))
		{
			if (filename == "renamed.txt")
				file_found = true;
			else
				assert(filename == "." || filename == "..");
		}
		assert(file_found);
	}


	assert(symlink("test/renamed.txt"sv, "test/symlink.txt"sv));
	{
		FileFd fd("test/renamed.txt"sv);
		assert(fd.writeContainer("Hi"sv));
	}
	{
		FileFd fd("test/renamed.txt"sv);
		assert(fd.read<std::string>(2).get() == "Hi");
	}

	assert(unlink("test/symlink.txt"sv));
	
	assert(link("test/renamed.txt"sv, "test/link.txt"sv));
	assert(stat("test/renamed.txt"sv).get().st_nlink == 2);
	assert(stat("test/link.txt"sv).get().st_nlink == 2);

	assert(unlink("test/link.txt"sv));
	assert(unlink("test/renamed.txt"sv));

	{
		DirFd dir("test");
		assert(dir);
		assert(dir.getFilenames().size() == 2); // . and ..
	}

	assert(chdir("test"sv));

	auto current_dir = getCurrentDir();
	assert(current_dir);
	assert(current_dir->find("/test") != std::string::npos);

	assert(chdir(".."sv));

	assert(rmdir("test"sv));
	assert(!rmdir("test"sv));

	assert(!chdir("test"sv));
}