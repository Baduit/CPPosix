#pragma once

#include <sys/types.h>
#include <unistd.h>

#include "Expected.hpp"

namespace Cpposix
{

inline pid_t getProcessId() { return ::getpid(); }
inline pid_t getParentProcessId() { return ::getppid(); }

inline Expected<Void> setProcessGroupId(pid_t pid, pid_t pgid)
{
	if (::setpgid(pid, pgid) == -1)
		return Error();
	else
		return Void();
}

Expected<pid_t> getProcessGroupId(pid_t pid)
{
	auto result = ::getpgid(pid);
	if (result == -1)
		return Error();
	else
		return result;
}


inline uid_t 	getRealUid() { return ::getuid(); }
inline uid_t 	getEffectiveUid() { return ::geteuid(); }

inline Expected<Void> setRealUid(uid_t  pid)
{
	if (::setuid(pid) == -1)
		return Error();
	else
		return Void();
}

inline Expected<Void> setEffectiveUid(uid_t  pid)
{
	if (::seteuid(pid) == -1)
		return Error();
	else
		return Void();
}

inline Expected<pid_t >	getSessionId(pid_t pid = 0)
{
	auto result = ::getsid(pid);
	if (result == -1)
		return Error();
	else
		return result;
}

inline Expected<pid_t>	newSessionId()
{
	auto result = ::setsid();
	if (result == -1)
		return Error();
	else
		return result;
}

}