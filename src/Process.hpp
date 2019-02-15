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

	private:
		pid_t	_pid;
};

namespace
{

std::vector<const char*>	svVectorToCStringVector(const std::vector<std::string_view>& sv_vector)
{
	std::vector<const char*> cstring_vector;
	for (auto sv: sv_vector)
	{
		if (sv[sv.size()])
			throw CpposixException("The string argument is not null terminated");
		cstring_vector.push_back(sv.data());
	}
	cstring_vector.push_back(nullptr);
	return cstring_vector;
}

}

inline Expected<Void>	exec(std::string_view filename, std::vector<std::string_view> argv)
{
	if (filename[filename.size()])
		throw CpposixException("The string argument is not null terminated");
	argv.insert(argv.begin(), filename);
	execvp(filename.data(),(char* const*) (svVectorToCStringVector(argv).data()));
	return Error();
}

inline Expected<Void>	exec(std::string_view filename, std::vector<std::string_view> argv, const std::vector<std::string_view>& arge)
{
	if (filename[filename.size()])
		throw CpposixException("The string argument is not null terminated");
	argv.insert(argv.begin(), filename);
	execvpe(filename.data(), (char* const*) (svVectorToCStringVector(argv).data()),(char* const*) (svVectorToCStringVector(arge).data()));
	return Error();
}

}