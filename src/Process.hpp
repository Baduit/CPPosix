#pragma once

namespace Cpposix
{
// do not forget nice(int)
// for exec functions, do not forget to try to add a end of scope guard, because exec family functions seems to break RAII
}