cmake_minimum_required(VERSION 3.3)
project(TOR_THE_ELEVAAAAATOOOOOR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp elevator_controll.cpp io.c network.cpp state_machine.cpp testElevContrl.cpp state_machine.h)
add_executable(Oving1 ${SOURCE_FILES})