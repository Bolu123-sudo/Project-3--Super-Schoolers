cmake_minimum_required(VERSION 3.15)
project(SuperSchoolers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# headers dir if you keep .hpp in include/
target_include_directories(SchoolSystem PUBLIC ${CMAKE_SOURCE_DIR}/include)

# main CLI app
add_executable(SchoolSystem
        src/main.cpp
        src/SchoolSystem.cpp
        src/Output.cpp
        src/regionfilter.cpp
        src/Ranking.cpp
        src/quickSelect.cpp

)


add_executable(schools_clean
        src/schools_clean.cpp
)
target_include_directories(schools_clean PUBLIC ${CMAKE_SOURCE_DIR}/include)
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
