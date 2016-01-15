cmake_minimum_required(VERSION 3.3)
project(Oving1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp)
add_executable(Oving1 ${SOURCE_FILES})