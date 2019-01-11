cmake_minimum_required(VERSION 3.0)

INCLUDE(CMakeForceCompiler) 
CMAKE_FORCE_C_COMPILER(gcc GNU)
CMAKE_FORCE_CXX_COMPILER(g++ GNU)

project(CPPosix)
include_directories(CPPosix PRIVATE ./src)
add_definitions(-std=c++17)


set(SRCS_EXPECTED test/test_expected.cpp)
add_executable(test_expected ${SRCS_EXPECTED})

set(SRCS_CONCEPT test/test_concept.cpp)
add_executable(test_concept ${SRCS_CONCEPT})
