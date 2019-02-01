#pragma once

#include <fcntl.h>

#include "Fd.hpp"
#include "Dir.hpp"

namespace Cpposix
{

using FileStat = struct stat;
using FileStatVfs = struct statvfs;

struct FileMode
{
	FileMode() = default;
	FileMode(mode_t m): mode(m) {}

	template <typename ...Args>
	FileMode(Args... args): mode((... | args)) {}

	mode_t mode;
};

struct FileFlags
{
	FileFlags() = default;
	FileFlags(int f): flags(f) {}

	template <typename ...Args>
	FileFlags(Args... args): flags((... | args)) {}

	int flags;
};

class FileFd: public Fd
{
	public:
		FileFd() = default;
		explicit FileFd(int fd): Fd(fd) {}
		FileFd(std::string_view filename, FileFlags flags)
		{
			if (filename[filename.size()])
				throw CpposixException("The string argument is not null terminated");
			_fd = ::open(filename.data(), flags.flags);
		}

		FileFd(std::string_view filename, FileFlags flags, FileMode creation_mode)
		{
			if (filename[filename.size()])
				throw CpposixException("The string argument is not null terminated");
			_fd = ::open(filename.data(), flags.flags, creation_mode.mode);
		}

		static Expected<FileFd>	create(std::string_view filename, FileMode creation_mode)
		{
			if (filename[filename.size()])
				throw CpposixException("The string argument is not null terminated");
			FileFd fd(::creat(filename.data(), creation_mode.mode));
			if (fd)
				return fd;
			else
				return Error();
		}

		Expected<FileStat>	stat()
		{
			FileStat s;
			off_t result = ::fstat(_fd, &s);
			if (result != -1)
				return s;
			else
				return Error();
		}

		Expected<FileStatVfs>	statVfs()
		{
			FileStatVfs s;
			off_t result = ::fstatvfs(_fd, &s);
			if (result != -1)
				return s;
			else
				return Error();
		}

		Expected<Void>	chmod(mode_t mode)
		{
			if (::fchmod(_fd, mode) != -1)
				return Void();
			else
				return Error();
		}

		Expected<Void>	chown(uid_t owner, gid_t group)
		{
			if (::fchown(_fd, owner, group) != -1)
				return Void();
			else
				return Error();
		}

		Expected<Void>	truncate(off_t length)
		{
			if (::ftruncate(_fd, length) != -1)
				return Void();
			else
				return Error();
		}

		Expected<off_t>	lseek(off_t  offset, int whence)
		{
			off_t result = ::lseek(_fd, offset, whence);
			if (result != -1)
				return result;
			else
				return Error();
		}

		Expected<long>	pathConf(int name)
		{	
			long result = ::fpathconf(_fd, name);
			if (result != -1)
				return result;
			else
				return Error();
		}
};

}