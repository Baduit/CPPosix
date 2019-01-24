cmake_minimum_required(VERSION 3.0)

project(CPPosix)
include_directories(CPPosix PRIVATE ./src)
add_definitions(-std=c++17 -Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith) 


set(SRCS_EXPECTED test/test_expected.cpp)
add_executable(test_expected ${SRCS_EXPECTED})

set(SRCS_CONCEPT test/test_concept.cpp)
add_executable(test_concept ${SRCS_CONCEPT})

set(SRCS_BUFFER test/test_buffer.cpp)
add_executable(test_buffer ${SRCS_BUFFER})

set(SRCS_FD test/test_fd.cpp)
add_executable(test_fd ${SRCS_FD})

set(SRCS_ENV test/test_env.cpp)
add_executable(test_env ${SRCS_ENV})