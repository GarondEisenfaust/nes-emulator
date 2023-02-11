#include "addressing-modes/AddressingModeContainer.h"

AddressingModeContainer::AddressingModeContainer(Processor6502* cpu)
    : abs(cpu),
      abx(cpu),
      aby(cpu),
      imm(cpu),
      imp(cpu),
      ind(cpu),
      izx(cpu),
      izy(cpu),
      rel(cpu),
      zp0(cpu),
      zpx(cpu),
      zpy(cpu) {}
