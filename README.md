# CPPosix
A modern C++ header-only library which encapsulate the C Posix library but with modern c++ features.

Requires C++20 because of concepts and ranges. Tested on Ubuntu with gcc on the trunk: gcc (cqwrteur) 10.0.0 20191220 (experimental).
To build with cmake you need a recent version of cmake. (tested with cmake 3.13.3)

TODO:
* A basic Documentation
* Improve the Dir part
* Improve the Env part
* Add some utility on the IO methods for the fd (getline, readExact, readAll)
