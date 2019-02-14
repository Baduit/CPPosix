#pragma once

#include <sys/types.h>
#include <unistd.h>

namespace Cpposix
{

// those functions can't fail, and it is just forwarding, no TU needed

inline pid_t getProcessusId() { return ::getpid(); }
inline pid_t getParentProcessusId() { return ::getppid(); }

}