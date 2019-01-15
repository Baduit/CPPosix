#pragma once

#include <unistd.h>

namespace Cpposix
{

class Fd
{
	public:
		Fd() = default;
		Fd(int fd): _fd(fd) {}

		virtual ~Fd() { close(); }

		operator bool () const { return fd != -1; }

		void assign(int fd) { _fd = fd; }
		void close()
		{
			if (*this)
				::close(_fd);
		}


	protected:
		int _fd = -1;
};

}