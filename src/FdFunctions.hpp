#pragma once

#include <chrono>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "Fd.hpp"
#include "PipeFd.hpp"

namespace Cpposix
{

template <typename FdType>
Expected<FdType>	dup(const FdType& fd)
{
	int result = ::dup(fd.getFdAsInt());
	if (result == -1)
		return Error();
	else
		return FdType(result);
}

inline Expected<Void> dup(const Fd& old_fd, const Fd& new_fd)
{
	if (::dup2(new_fd.getFdAsInt(), old_fd.getFdAsInt()) == -1)
		return Error();
	else
		return Void();
}

inline Expected<Void> dup(const Fd& old_fd, const Fd& new_fd, FdFlags flags)
{
	if (::dup3(new_fd.getFdAsInt(), old_fd.getFdAsInt(), flags.flags) == -1)
		return Error();
	else
		return Void();
}

// redirect old_fd to new_fd
class RedirectFd
{
	public:
		RedirectFd(Fd& old_fd, Fd& new_fd):
			_old_fd(old_fd),
			_dup_old_fd(std::move(dup(old_fd).get()))
		{
			dup(old_fd, new_fd);
		}

		RedirectFd(Fd& old_fd, Fd& new_fd, FdFlags flags):
			_old_fd(old_fd),
			_dup_old_fd(std::move(dup(old_fd).get()))
		{
			dup(old_fd, new_fd, flags.flags);
		}

		~RedirectFd()
		{
			dup(_old_fd, _dup_old_fd);
		}

	private:
		Fd&	_old_fd;
		Fd	_dup_old_fd;
};


struct FdEvent
{
	std::vector<FdRef>	read;
	std::vector<FdRef>	write;
	std::vector<FdRef>	except;
};


/* Helper impl */
using Timeval = struct timeval;

inline int	setVectorToFdSet(fd_set& fds, const std::vector<FdRef>& v)
{
	FD_ZERO(&fds);
	int max = 0;
	for (auto f: v)
	{
		if (f.get().getFdAsInt() > max)
			max = f.get().getFdAsInt();
		FD_SET(f.get().getFdAsInt(), &fds);
	}
	return max;
}

struct FdSets
{
	FdSets(const FdEvent& fds_to_watch)
	{
		max = setVectorToFdSet(rfds, fds_to_watch.read);

		int max_w = setVectorToFdSet(wfds, fds_to_watch.write);
		max = (max > max_w) ? max : max_w;

		int max_e = setVectorToFdSet(efds, fds_to_watch.except);
		max = (max > max_e) ? max : max_e;

		++max;
	}

	fd_set rfds;
	fd_set wfds;
	fd_set efds;
	int max = 0;
};

inline std::vector<FdRef>	getVectorResultFromFdSet(const std::vector<FdRef>& v, fd_set fds)
{
	std::vector<FdRef> activ_fds;

	for (auto f: v)
	{
		if (FD_ISSET(f.get().getFdAsInt(), &fds))
			activ_fds.push_back(f);
	}

	return activ_fds;
}

inline FdEvent		extractSelectResult(const FdEvent& fds_to_watch, const FdSets& fd_sets)
{
	FdEvent	fd_event;

	fd_event.read = getVectorResultFromFdSet(fds_to_watch.read, fd_sets.rfds);
	fd_event.write = getVectorResultFromFdSet(fds_to_watch.write, fd_sets.wfds);
	fd_event.except = getVectorResultFromFdSet(fds_to_watch.except, fd_sets.efds);

	return fd_event;
}

template< class Rep, class Period >
Timeval	durationToTimeval(const std::chrono::duration<Rep, Period> duration)
{
	Timeval t;
	auto int_s = std::chrono::duration_cast<std::chrono::seconds>(duration);
	t.tv_sec = int_s.count();
	auto int_us = std::chrono::duration_cast<std::chrono::microseconds>(duration - int_s);
	t.tv_usec = int_us.count();
	return t;
}

/* Helper impl END*/

inline Expected<FdEvent>	fdSelect(const FdEvent& fds_to_watch)
{
	FdSets fd_sets(fds_to_watch);

	auto result = ::select(fd_sets.max, &(fd_sets.rfds), &(fd_sets.wfds), &(fd_sets.efds), nullptr);
	if (result == -1)
		return Error();

	return extractSelectResult(fds_to_watch, fd_sets);
}

template< class Rep, class Period>
Expected<FdEvent>	fdSelect(const FdEvent& fds_to_watch, const std::chrono::duration<Rep, Period> sleep_duration)
{

	Timeval t = durationToTimeval(sleep_duration);
	FdSets fd_sets(fds_to_watch);

	auto result = ::select(fd_sets.max, &(fd_sets.rfds), &(fd_sets.wfds), &(fd_sets.efds), &t);
	if (result == -1)
		return Error();

	return extractSelectResult(fds_to_watch, fd_sets);
}

}