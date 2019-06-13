cmake_minimum_required(VERSION 3.0)

project(CPPosix)
add_definitions(-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(CPPosix PRIVATE ./src)

# Tests
enable_testing()

add_executable(expected.test test/test_expected.cpp)
add_test(test_expected expected.test)

add_executable(concept.test test/test_concept.cpp)
add_test(test_concept concept.test)

add_executable(buffer.test test/test_buffer.cpp)
add_test(test_buffer buffer.test)

add_executable(fd.test test/test_fd.cpp)
add_test(test_fd fd.test)

add_executable(env.test test/test_env.cpp)
add_test(test_env env.test)

add_executable(filesystem.test test/test_filesystem.cpp)
add_test(test_filesystem filesystem.test)

add_executable(process.test test/test_process.cpp)
add_test(test_process process.test)

set(SRC_EXAMPLE example/main.cpp)
add_executable(example.test ${SRC_EXAMPLE})