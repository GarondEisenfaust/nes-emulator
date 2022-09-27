#pragma once
#include <stdexcept>

#define RAM_START 0x0000
#define RAM_END 0xFFFF
#define STACK_BEGIN 0x100

#define UNDEFINED 0x00

#define NOT_IMPLEMENTED_EXCEPTION std::runtime_error("This function has not been implemented!");
