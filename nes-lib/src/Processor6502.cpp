#include "Processor6502.h"
#include "Bus.h"
#include "Definitions.h"
#include "Util.h"
#include "fmt/format.h"
#include <cstdint>
#include <iostream>

// Constructor
Processor6502::Processor6502() : addressingModes(this), opcodes(this) {
  lookup = std::move(MakeLookupTable(addressingModes, opcodes));
}

void Processor6502::ConnectBus(Bus* bus) {
  mBus = bus;
  mBus->mCpu = this;
}

uint8_t Processor6502::Read(uint16_t addr) { return mBus->CpuRead(addr); }
uint8_t Processor6502::PopFromStack() {
  stackPointer++;
  return Read(STACK_BEGIN + stackPointer);
}

void Processor6502::Write(uint16_t addr, uint8_t data) { mBus->CpuWrite(addr, data); }
void Processor6502::PushToStack(uint8_t data) {
  Write(STACK_BEGIN + stackPointer, data);
  stackPointer--;
}

uint8_t Processor6502::Fetch() {
  if (!(lookup->at(opcode).addrMode == &addressingModes.imp)) {
    fetched = Read(addrAbs);
  }
  return fetched;
}

void Processor6502::Clock() {
  if (cycles == 0) {
    opcode = Read(pc);
    status.u = true;
    pc++;

    auto instruction = lookup->at(opcode);
    cycles = lookup->at(opcode).cycles;

    auto additionalCycle1 = (*lookup->at(opcode).addrMode)();
    auto additionalCycle2 = (*lookup->at(opcode).opcode)();

    cycles += additionalCycle1 && additionalCycle2;
    status.u = true;
  }
  cycles--;
}

void Processor6502::Reset() {
  addrAbs = 0xFFFC;
  uint16_t lo = Read(addrAbs + 0);
  uint16_t hi = Read(addrAbs + 1);

  pc = (hi << 8) | lo;
  // nestest
  // pc = 0xC000;

  a = 0;
  x = 0;
  y = 0;
  stackPointer = 0xFD;
  status.reg = 0x00;
  status.u = true;

  addrRel = 0x0000;
  addrAbs = 0x0000;
  fetched = 0x00;

  cycles = 8;
}

bool Processor6502::BranchIf(bool condition) {
  if (condition) {
    cycles++;
    addrAbs = pc + addrRel;

    if ((addrAbs & 0xFF00) != (pc & 0xFF00)) {
      cycles++;
    }

    pc = addrAbs;
  }
  return 0;
}

void Processor6502::Interrupt(uint16_t address, uint8_t numCycles) {
  PushToStack((pc >> 8) & 0x00FF);
  PushToStack(pc & 0x00FF);

  status.b = false;
  status.u = true;
  status.i = true;
  PushToStack(status.reg);

  addrAbs = address;
  uint16_t lo = Read(addrAbs + 0);
  uint16_t hi = Read(addrAbs + 1);
  pc = (hi << 8) | lo;

  cycles = numCycles;
}

void Processor6502::Interrupt() {
  if (status.i != 0) {
    return;
  }

  Interrupt(0xFFFE, 7);
}

void Processor6502::NonMaskableInterrupt() { Interrupt(0xFFFA, 8); }

std::map<uint16_t, std::string> Processor6502::Disassemble(uint16_t begin, uint16_t end) {
  uint32_t current = begin;
  std::map<uint16_t, std::string> disassembledCode;

  while (current <= end) {
    auto line = current;
    auto opcode = mBus->CpuRead(current, true);
    auto& instruction = lookup->at(opcode);
    current++;

    auto name = instruction.opcode->Name();
    auto addressingMode = instruction.addrMode->Disassemble(current);
    disassembledCode[line] = fmt::format("{:#06x}: {} {}", line, name, addressingMode);
  }
  return disassembledCode;
}
