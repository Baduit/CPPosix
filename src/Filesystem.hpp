#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "FileFd.hpp"

namespace Cpposix
{

inline Expected<Void>		rename(std::string_view old_name, std::string_view new_name)
{
	check_string_view_null_terminated(old_name);
	check_string_view_null_terminated(new_name);

	if (::rename(old_name.data(), new_name.data()) != -1)
		return Void();
	else
		return Error();
}

inline Expected<Void>		rmdir(std::string_view dirname)
{
	check_string_view_null_terminated(dirname);

	if (::rmdir(dirname.data()) != -1)
		return Void();
	else
		return Error();
}

inline Expected<Void>		mkdir(std::string_view dirname, FileMode mode = all_permissions)
{
	check_string_view_null_terminated(dirname);

	if (::mkdir(dirname.data(), mode.mode) != -1)
		return Void();
	else
		return Error();
}

inline Expected<Void>		chdir(std::string_view dirname)
{
	check_string_view_null_terminated(dirname);

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
	check_string_view_null_terminated(filename);

	if (::unlink(filename.data()) != -1)
		return Void();
	else
		return Error();
}

inline Expected<Void>		link(std::string_view target, std::string_view linkname)
{
	check_string_view_null_terminated(target);
	check_string_view_null_terminated(linkname);

	if (::link(target.data(), linkname.data()) != -1)
		return Void();
	else
		return Error();
}

inline Expected<Void>		symlink(std::string_view target, std::string_view linkname)
{
	check_string_view_null_terminated(target);
	check_string_view_null_terminated(linkname);

	if (::symlink(target.data(), linkname.data()) != -1)
		return Void();
	else
		return Error();
}

inline Expected<Void>		mkfifo(std::string_view filename, FileMode mode)
{
	check_string_view_null_terminated(filename);

	if (::mkfifo(filename.data(), mode.mode) != -1)
		return Void();
	else
		return Error();
}

inline Expected<FileStat>	stat(std::string_view filename)
{
	check_string_view_null_terminated(filename);

	FileStat s;
	off_t result = ::stat(filename.data(), &s);
	if (result != -1)
		return s;
	else
		return Error();
}

inline Expected<FileStatVfs>	statVfs(std::string_view filename)
{
	check_string_view_null_terminated(filename);

	FileStatVfs s;
	off_t result = ::statvfs(filename.data(), &s);
	if (result != -1)
		return s;
	else
		return Error();
}

inline Expected<Void>	chmod(std::string_view filename, mode_t mode)
{	
	check_string_view_null_terminated(filename);

	if (::chmod(filename.data(), mode) != -1)
		return Void();
	else
		return Error();
}

inline Expected<Void>	chown(std::string_view filename, uid_t owner, gid_t group)
{	
	check_string_view_null_terminated(filename);

	if (::chown(filename.data(), owner, group) != -1)
		return Void();
	else
		return Error();
}

inline Expected<Void>	truncate(std::string_view filename, off_t length)
{	
	check_string_view_null_terminated(filename);

	if (::truncate(filename.data(), length) != -1)
		return Void();
	else
		return Error();
}

inline Expected<long>	pathConf(std::string_view filename, int name)
{	
	check_string_view_null_terminated(filename);

	long result = ::pathconf(filename.data(), name);
	if (result != -1)
		return result;
	else
		return Error();
}

}