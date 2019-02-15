#pragma once

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "Expected.hpp"
#include "PipeFd.hpp"

namespace Cpposix
{


class ChildProcess
{
	public:
		// template
		template <typename Cb, class... Args>
		ChildProcess(Cb&& cb, Args&&... args)
		{
			_pid = fork();
			if (_pid == -1)
				throw CpposixException("ChildProcess: Error while forking.");

			if (_pid == 0)
			{
				if constexpr (std::is_same<int, typename std::result_of<Cb(Args&&...)>::type>::value)
				{
					exit(cb(std::forward<Args...>(args)...));
				}
				else
				{
					cb(std::forward<Args...>(args)...);
					exit(EXIT_SUCCESS);
				}
			}
		}

		Expected<Void>	kill()
		{
			if (_pid == 0)
				return Error();

			if (::kill(_pid, SIGKILL) == -1)
				return Error();

			return Void();
		}

		Expected<int>	wait(int options = 0)
		{
			if (_pid == 0)
				return Error();
			
			int	status = 0;
			if (::waitpid(_pid, &status, options) == -1)
				return Error();
			return WEXITSTATUS(status);
		}

		pid_t	getChildPid() const { return _pid; }
	public:
		Pipe	communication;
	
	private:
		pid_t	_pid;
};

// for exec functions, do not forget to try to add a end of scope guard, because exec family functions seems to break RAII
}