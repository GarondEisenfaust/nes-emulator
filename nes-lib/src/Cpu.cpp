#include "Cpu.h"
#include "Bus.h"
#include "Definitions.h"
#include <cstdint>
#include <iostream>

Cpu::Cpu() : addressingModes(this), opcodes(this) { lookup = std::move(MakeLookupTable(addressingModes, opcodes)); }

void Cpu::ConnectBus(Bus* bus) {
  mBus = bus;
  mBus->mCpu = this;
}

uint8_t Cpu::ApuRead(uint16_t addr) {
  cycles += 4;
  return Read(addr);
}

uint8_t Cpu::Read(uint16_t addr) { return mBus->CpuRead(addr); }
uint16_t Cpu::ReadTwoBytes(uint16_t addr) {
  uint16_t low = Read(addr);
  uint16_t high = Read(addr + 1);
  return static_cast<uint16_t>(high << 8) | low;
}

void Cpu::Write(uint16_t addr, uint8_t data) { mBus->CpuWrite(addr, data); }

uint8_t Cpu::PopFromStack() {
  stackPointer++;
  return Read(STACK_BEGIN + stackPointer);
}

void Cpu::PushToStack(uint8_t data) {
  Write(STACK_BEGIN + stackPointer, data);
  stackPointer--;
}

uint8_t Cpu::Fetch() {
  if (!(lookup->at(opcode).addrMode == &addressingModes.imp)) {
    fetched = Read(addrAbs);
  }
  return fetched;
}

void Cpu::Clock() {
  if (cycles == 0) {
    opcode = Read(pc);
    status.u = true;
    pc++;

    auto instruction = lookup->at(opcode);
    cycles = instruction.cycles;

    auto pageCrossed = (*instruction.addrMode)();
    (*instruction.opcode)();

    cycles += pageCrossed && instruction.additionalCycleOnPageCrossed;
    status.u = true;
  }
  cycles--;
}

void Cpu::Reset() {
  pc = ReadTwoBytes(0xFFFC);
  // nestest
  // pc = ReadTwoBytes(0xC000);

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

void Cpu::BranchIf(bool condition) {
  if (condition) {
    cycles++;
    addrAbs = pc + addrRel;

    if ((addrAbs & 0xFF00) != (pc & 0xFF00)) {
      cycles++;
    }

    pc = addrAbs;
  }
}

void Cpu::Interrupt(uint16_t address, uint8_t numCycles) {
  PushToStack((pc >> 8) & 0x00FF);
  PushToStack(pc & 0x00FF);

  status.b = false;
  status.u = true;
  PushToStack(status.reg);
  status.i = true;

  addrAbs = address;
  pc = ReadTwoBytes(addrAbs);

  cycles = numCycles;
}

void Cpu::ExecuteInterrupt() {
  if (status.i) {
    return;
  }

  Interrupt(0xFFFE, 7);
}

void Cpu::ExecuteNonMaskableInterrupt() { Interrupt(0xFFFA, 8); }
