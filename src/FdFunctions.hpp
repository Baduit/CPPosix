#pragma once

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

Expected<Void> dup(Fd& old_fd, Fd& new_fd)
{
	if (::dup2(old_fd.getFdAsInt(), new_fd.getFdAsInt()) == -1)
		return Error();
	else
		return Void();
}

Expected<Void> dup(Fd& old_fd, Fd& new_fd, FdFlags flags)
{
	if (::dup3(old_fd.getFdAsInt(), new_fd.getFdAsInt(), flags.flags) == -1)
		return Error();
	else
		return Void();
}

class RedirectFd
{
	public:
		RedirectFd(Fd& old_fd, Fd& new_fd):
			_remember_fd(dup(old_fd)),
			_old_fd(old_fd)
		{
			dup(old_fd, new_fd);
		}

		RedirectFd(Fd& old_fd, Fd& new_fd, FdFlags flags):
			_remember_fd(dup(old_fd)),
			_old_fd(old_fd)
		{
			dup(old_fd, new_fd, flags.flags);
		}

		~RedirectFd()
		{
			dup(_remember_fd, _old_fd);
		}

	private:
		Fd	_remember_fd;
		Fd&	_old_fd;
};

// poll(vector<pair<fd&, event to watch>>, optional timeout) -> vector<pair<Fd&, event catched>>
// remplacer les vector de pair par une structure encapsulant ça

// select (vector<Fd&> read, vector<Fd&> write, vector<Fd&> signal, optional_timeout) -> structure avec les 3 
// remplacer les 3 arguments par une structure contenant les 3

// mouai bof pas super, ça retient rien, si dans une boucle tout est reset et tout

/*
class FdEvent
{
	public:

	private:
		
};
*/

}