#pragma once

#include <sys/types.h>
#include <unistd.h>

namespace Cpposix
{

inline pid_t getProcessusId() { return ::getpid(); }
inline pid_t getParentProcessusId() { return ::getppid(); }

}