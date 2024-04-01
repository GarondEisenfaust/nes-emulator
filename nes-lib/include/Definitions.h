#pragma once
#include <stdexcept>

#define STACK_BEGIN 0x0100
#define UNDEFINED 0x00
#define WIDTH 1024
#define HEIGHT 960

#define CPU_CLOCK_SPEED 1789773.0

#define NOT_IMPLEMENTED_EXCEPTION std::runtime_error("This function has not been implemented!");
