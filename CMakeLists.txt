cmake_minimum_required(VERSION 3.0)

project(CPPosix)
include_directories(CPPosix PRIVATE ./src)
add_definitions(-std=c++17 -Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith) 


set(SRCS_EXPECTED test/test_expected.cpp)
add_executable(expected.test ${SRCS_EXPECTED})

set(SRCS_CONCEPT test/test_concept.cpp)
add_executable(concept.test ${SRCS_CONCEPT})

set(SRCS_BUFFER test/test_buffer.cpp)
add_executable(buffer.test ${SRCS_BUFFER})

set(SRCS_FD test/test_fd.cpp)
add_executable(fd.test ${SRCS_FD})

set(SRCS_ENV test/test_env.cpp)
add_executable(env.test ${SRCS_ENV})

set(SRCS_FILESYSTEM test/test_filesystem.cpp)
add_executable(filesystem.test ${SRCS_FILESYSTEM})

set(SRCS_TERMIOS test/test_termios.cpp)
add_executable(termios.test ${SRCS_TERMIOS})

set(SRCS_ID test/test_id.cpp)
add_executable(id.test ${SRCS_ID})