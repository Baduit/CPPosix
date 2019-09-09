[![Build Status](https://img.shields.io/endpoint.svg?url=https%3A%2F%2Factions-badge.atrox.dev%2Fbaduit%2FCPPosix%2Fbadge%3Fref%3Dmaster&style=flat)](https://actions-badge.atrox.dev/baduit/CPPosix/goto?ref=master)
# CPPosix
A modern C++ header-only library which encapsulate the C Posix library but with modern c++ features.

TODO:
* Documentation
* Improve the Dir part
* Improve the Env part
* A real readme not
* Hide the IO complexity in the Fd class it makes it really difficult to read
* Correct the fact that a string_view is not considered as a container because std::has_unique_object_representations<std::string_view>::value is true
