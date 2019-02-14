#pragma once

#include <optional>
#include <termios.h>
#include <unistd.h>

#include "Fd.hpp"

namespace Cpposix
{

// http://man7.org/linux/man-pages/man3/termios.3.html

using CTermios = struct termios;

struct Termios
{
	Termios(Fd& fd)
	{
		if (tcgetattr(fd.getFdAsInt(), &termios) == -1)
			throw CpposixException("Error tcgetattr");
	}

	void			makeRaw() { cfmakeraw(&termios); }

	Expected<Void>	setInputSpeed(speed_t speed)
	{
		if (cfsetispeed(&termios, speed) == -1)
			return Error();
		else
			return Void();
	}

	Expected<Void>	seOutputSpeed(speed_t speed)
	{
		if (cfsetospeed(&termios, speed) == -1)
			return Error();
		else
			return Void();
	}

	speed_t	getInputSpeed() { return cfgetispeed(&termios); }
	speed_t	geOutputSpeed() { return cfgetispeed(&termios); }

	CTermios			termios;
};

Expected<Termios> getAttr(Fd& fd)
{
	try
	{
		return Termios(fd);
	}
	catch (CpposixException&)
	{
		return Error();
	}
}

Expected<Void> setAttr(Fd& fd, const Termios& termios, int optional_actions = 0)
{
	if (tcsetattr(fd, optional_actions, &(termios.termios)) == -1)
		return Error();
	else
		return Void();
}

Expected<Void> sendBreak(Fd& fd, int duration)
{
	if (tcsendbreak(fd, duration) == -1)
		return Error();
	else
		return Void();
}

Expected<Void> drain(Fd& fd)
{
	if (tcdrain(fd) == -1)
		return Error();
	else
		return Void();
}

Expected<Void> flush(Fd& fd, int queue_selector)
{
	if (tcflush(fd, queue_selector) == -1)
		return Error();
	else
		return Void();
}

Expected<Void> flow(Fd& fd, int action)
{
	if (tcflow(fd, action) == -1)
		return Error();
	else
		return Void();
}

class TermiosHelper
{
	public:
		TermiosHelper(Fd& fd):
			_fd(fd),
			_old_termios(fd)
		{}

		TermiosHelper(Fd&& fd):
			_own_fd(std::move(fd)),
			_fd(*_own_fd),
			_old_termios(fd)
		{}

		~TermiosHelper()
		{
			setAttr(_old_termios);
		}

		Expected<Termios> getAttr() { return Cpposix::getAttr(_fd); }

       	Expected<Void> setAttr(const Termios& termios, int optional_actions = 0) { return Cpposix::setAttr(_fd, termios, optional_actions); }

       	Expected<Void> sendBreak(int duration) { return Cpposix::sendBreak(_fd, duration); }

       	Expected<Void> drain() { return Cpposix::drain(_fd); }

       	Expected<Void> flush(int queue_selector) { return Cpposix::flush(_fd, queue_selector); }

       	Expected<Void> flow(int action) { return Cpposix::flow(_fd, action); }


	private:
		std::optional<Fd>	_own_fd; // we can own the fd, but it is not necessary
		Fd&					_fd;
		Termios				_old_termios;
};

}