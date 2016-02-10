cmake_minimum_required(VERSION 3.3)
project(Untitled_Folder)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES elevator_controll.cpp io.c main.cpp state_machine.cpp testElevContrl.cpp Network.cpp)
add_executable(Untitled_Folder ${SOURCE_FILES})