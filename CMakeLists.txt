cmake_minimum_required(VERSION 3.0) # update this please

project(CPPosix)
add_definitions(-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -fconcepts) 

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(CPPosix PRIVATE ./src)

# Tests
enable_testing()

#add_executable(expected.test test/test_expected.cpp)
#add_test(test_expected expected.test)

add_executable(fd.test test/test_fd.cpp)
add_test(test_fd fd.test)

#add_executable(env.test test/test_env.cpp)
#add_test(test_env env.test)

#add_executable(filesystem.test test/test_filesystem.cpp)
#add_test(test_filesystem filesystem.test)

#add_executable(process.test test/test_process.cpp)
#add_test(test_process process.test)

#add_executable(test_check_string_disable_macro.test test/test_check_string_disable_macro.cpp)
#add_test(test_check_string_disable_macro test_check_string_disable_macro.test)

#set(SRC_EXAMPLE example/main.cpp)
#add_executable(example.test ${SRC_EXAMPLE})