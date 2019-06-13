#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "FileFd.hpp"

namespace Cpposix
{

inline Expected<Void>		rename(std::string_view old_name, std::string_view new_name)
{
	if (old_name[old_name.size()] || new_name[new_name.size()])
		throw CpposixException("The string argument is not null terminated");

	if (::rename(old_name.data(), new_name.data()) != -1)
		return Void();
	else
		return Error();
}

inline Expected<Void>		rmdir(std::string_view dirname)
{
	if (dirname[dirname.size()])
		throw CpposixException("The string argument is not null terminated");

	if (::rmdir(dirname.data()) != -1)
		return Void();
	else
		return Error();
}

inline Expected<Void>		mkdir(std::string_view dirname, FileMode mode = all_permissions)
{
	if (dirname[dirname.size()])
		throw CpposixException("The string argument is not null terminated");

	if (::mkdir(dirname.data(), mode.mode) != -1)
		return Void();
	else
		return Error();
}

inline Expected<Void>		chdir(std::string_view dirname)
{
	if (dirname[dirname.size()])
		throw CpposixException("The string argument is not null terminated");

	if (::chdir(dirname.data()) != -1)
		return Void();
	else
		return Error();
}

inline Expected<std::string>	getCurrentDir()
{
	char *dirname = get_current_dir_name();
	if (dirname == NULL)
		return Error();
	std::string result(dirname);
	free(dirname);
	return result;
}

inline Expected<Void>		unlink(std::string_view filename)
{
	if (filename[filename.size()])
		throw CpposixException("The string argument is not null terminated");

	if (::unlink(filename.data()) != -1)
		return Void();
	else
		return Error();
}

inline Expected<Void>		link(std::string_view target, std::string_view linkname)
{
	if (target[target.size()] || linkname[linkname.size()])
		throw CpposixException("The string argument is not null terminated");

	if (::link(target.data(), linkname.data()) != -1)
		return Void();
	else
		return Error();
}

inline Expected<Void>		symlink(std::string_view target, std::string_view linkname)
{
	if (target[target.size()] || linkname[linkname.size()])
		throw CpposixException("The string argument is not null terminated");

	if (::symlink(target.data(), linkname.data()) != -1)
		return Void();
	else
		return Error();
}

inline Expected<Void>		mkfifo(std::string_view filename, FileMode mode)
{
	if (filename[filename.size()])
		throw CpposixException("The string argument is not null terminated");

	if (::mkfifo(filename.data(), mode.mode) != -1)
		return Void();
	else
		return Error();
}

inline Expected<FileStat>	stat(std::string_view filename)
{
	if (filename[filename.size()])
		throw CpposixException("The string argument is not null terminated");

	FileStat s;
	off_t result = ::stat(filename.data(), &s);
	if (result != -1)
		return s;
	else
		return Error();
}

inline Expected<FileStatVfs>	statVfs(std::string_view filename)
{
	if (filename[filename.size()])
		throw CpposixException("The string argument is not null terminated");

	FileStatVfs s;
	off_t result = ::statvfs(filename.data(), &s);
	if (result != -1)
		return s;
	else
		return Error();
}

inline Expected<Void>	chmod(std::string_view filename, mode_t mode)
{	
	if (filename[filename.size()])
		throw CpposixException("The string argument is not null terminated");

	if (::chmod(filename.data(), mode) != -1)
		return Void();
	else
		return Error();
}

inline Expected<Void>	chown(std::string_view filename, uid_t owner, gid_t group)
{	
	if (filename[filename.size()])
		throw CpposixException("The string argument is not null terminated");

	if (::chown(filename.data(), owner, group) != -1)
		return Void();
	else
		return Error();
}

inline Expected<Void>	truncate(std::string_view filename, off_t length)
{	
	if (filename[filename.size()])
		throw CpposixException("The string argument is not null terminated");

	if (::truncate(filename.data(), length) != -1)
		return Void();
	else
		return Error();
}

inline Expected<long>	pathConf(std::string_view filename, int name)
{	
	if (filename[filename.size()])
		throw CpposixException("The string argument is not null terminated");

	long result = ::pathconf(filename.data(), name);
	if (result != -1)
		return result;
	else
		return Error();
}

}