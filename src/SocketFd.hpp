#pragma once

#include <sys/types.h>
#include <sys/socket.h>

#include "Fd.hpp"

namespace Cpposix
{

/*
** Finally i won't do the socket fd, because other libraries, like boost asio (which also have unix socket), already did an excellent job
** encapsulating them perfectly and in a portable way, and sockets are not really posix specifics.
*/

class SocketFd: public Fd
{
	public:
		SocketFd() = default;
		SocketFd(int fd): Fd(fd) {}

		SocketFd(int domain, int type, int protocol): Fd(::socket(domain = AF_INET, type = SOCK_STREAM, protocol = 0)) {}

};

}