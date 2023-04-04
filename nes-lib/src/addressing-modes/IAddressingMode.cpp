#include "addressing-modes/IAddressingMode.h"
#include "Bus.h"
#include "Cpu.h"

IAddressingMode::IAddressingMode(Cpu* cpu) : mCpu(cpu) {}
