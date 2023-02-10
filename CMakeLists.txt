cmake_minimum_required(VERSION 3.24)
project(TEST2)

set(CMAKE_CXX_STANDARD 17)

add_executable(TEST2
        main.cpp)

set(BOOST_ROOT /usr/include/boost)
FIND_PACKAGE(Boost 1.4 COMPONENTS mpi serialization REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(TEST2 ${Boost_LIBRARIES})