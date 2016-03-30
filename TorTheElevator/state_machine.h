#pragma once

#include <thread>
#include <mutex>
#include <atomic>
#include <unistd.h>
#include "main_include.h"
#include "elevator_controll.h"

void state_machine(const bool **requestMatrix, std::atomic<int>& finishedRequest, std::atomic<int>& latestFloor);
int motor_dir_to_matrix_dir(int motorDir);
bool check_stop(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int currentFloor,int moveDir);
bool request_on_floor(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int currentFloor);
bool request_in_dir(const bool requestMatrix[N_FLOORS][REQUEST_MATRIX_WIDTH], int moveDir,int currentFloor);