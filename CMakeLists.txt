cmake_minimum_required(VERSION 3.15)
project(SuperSchoolers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")


include_directories(src)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(SuperSchoolers
        src/main.cpp
        src/SchoolSystem.cpp
        src/BTree.h
        src/School.h
        src/MaxHeap.h
        src/SchoolSystem.h
)