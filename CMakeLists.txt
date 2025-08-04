cmake_minimum_required(VERSION 3.22)
project(Project3)

set(CMAKE_CXX_STANDARD 17)


set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )



include_directories(src)

add_executable(Project3
        src/main.cpp
        src/BTree.h
        src/MaxHeap.h
        src/School.h
        src/SchoolSystem.h
        src/SchoolSystem.cpp
)
