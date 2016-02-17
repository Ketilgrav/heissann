cmake_minimum_required(VERSION 3.3)
project(Oving6)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Network.cpp)
add_executable(Oving6 ${SOURCE_FILES})