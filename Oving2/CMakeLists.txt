cmake_minimum_required(VERSION 3.3)
project(Oving2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp semaphore.cpp)
add_executable(Oving2 ${SOURCE_FILES})