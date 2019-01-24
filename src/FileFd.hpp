#pragma once

#include "Fd.hpp"
#include "Dir.hpp"

namespace Cpposix
{

struct FileMode
{
	FileMode() = default;
	FileMode(mode_t m): mode(m) {}
	// add variadics template constro

	mode_t mode;
};

struct FileFlags
{
	FileFlags() = default;
	FileFlags(int f): flags(f) {}
	// add variadic template constro

	int flags;
};

class FileFd: public Fd
{
	public:
		FileFd() = default;
		explicit FileFd(int fd): Fd(fd) {}

		// constr open
		// constr openAt

		// static create

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

		Expected<Void>	chmown(uid_t owner, gid_t group)
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
};

}