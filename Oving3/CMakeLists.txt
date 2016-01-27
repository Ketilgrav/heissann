cmake_minimum_required(VERSION 3.3)
project(Oving3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp TCP.cpp UDP.cpp torsteik.cpp)
add_executable(Oving3 ${SOURCE_FILES})