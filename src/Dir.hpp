#pragma once

#include <sys/types.h>
#include <dirent.h>

#include <vector>
#include <string>

#include "Expected.hpp"
#include "Fd.hpp"

namespace Cpposix
{

using FileStat = struct stat;
using FileStatVfs = struct statvfs;
using Dirent = struct dirent;

class DirFd
{
	public:
		DirFd() = default;

		DirFd(std::string_view dirname)
		{
			if (dirname[dirname.size()])
				throw CpposixException("The string argument is not null terminated");
			_dir = opendir(dirname.data());
			reload();
		}
		
		DirFd(const Fd& fd)
		{
			_dir = fdopendir(fd.getFdAsInt());
			reload();
		}

		DirFd(const DirFd&) = delete;
		DirFd(DirFd&& other)
		{
			close();
			_dir = other._dir;
			other._dir = nullptr;
		}

		virtual ~DirFd()
		{
			close();
		}

		DirFd& operator=(const DirFd&) = delete;
		DirFd& operator=(DirFd&& other)
		{
			close();
			_dir = other._dir;
			other._dir = nullptr;
			return *this;
		}
		
		void	close()
		{
			if (*this)
				closedir(_dir);
		}

		operator bool() const { return _dir; }

		// this could be optimized by creating a custom iterator over the readir
		void reload()
		{
			if (!*this)
				throw CpposixException("The dir is NULL.");
			rewinddir(_dir);
			Dirent* dir_entry = nullptr;
			while ((dir_entry = readdir(_dir)))
				_files.push_back(dir_entry->d_name);
		}

		auto	begin()	{ return _files.begin(); }
		auto	cbegin() const { return _files.cbegin(); }
		auto	end() { return _files.end(); }
		auto	cend() const { return _files.cend(); }

		auto	rbegin()	{ return _files.rbegin(); }
		auto	crbegin() const { return _files.crbegin(); }
		auto	rend() { return _files.rend(); }
		auto	crend() const { return _files.crend(); }

		const std::vector<std::string>&	getFilenames() const { return _files; }
		
		DIR*	getHandle() { return _dir; }

		Expected<FileStat>	stat()
		{
			if (!*this)
				throw CpposixException("The dir is NULL.");

			FileStat s;
			off_t result = ::fstat(dirfd(_dir), &s);
			if (result != -1)
				return s;
			else
				return Error();
		}

		Expected<FileStatVfs>	statVfs()
		{
			if (!*this)
				throw CpposixException("The dir is NULL.");

			FileStatVfs s;
			off_t result = ::fstatvfs(dirfd(_dir), &s);
			if (result != -1)
				return s;
			else
				return Error();
		}

		Expected<Void>	chmod(mode_t mode)
		{
			if (!*this)
				throw CpposixException("The dir is NULL.");

			if (::fchmod(dirfd(_dir), mode) != -1)
				return Void();
			else
				return Error();
		}

		Expected<Void>	chown(uid_t owner, gid_t group)
		{
			if (!*this)
				throw CpposixException("The dir is NULL.");

			if (::fchown(dirfd(_dir), owner, group) != -1)
				return Void();
			else
				return Error();
		}

	protected:
		DIR*						_dir = nullptr;
		std::vector<std::string>	_files;
};

}