#pragma once

#include "Fd.hpp"

namespace Cpposix
{

class SocketFd: public Fd
{
	public:
		SocketFd() = default;
		SocketFd(int fd): Fd(fd) {}

		// constr accept

		// tout le blabla des sockets
};

}