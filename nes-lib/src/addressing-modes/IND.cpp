#include "addressing-modes/IND.h"
#include "Bus.h"
#include "Cpu.h"
#include "fmt/format.h"

IND::IND(Cpu* cpu) : IAddressingMode(cpu) {}

bool IND::operator()() {
  uint16_t ptrLow = mCpu->Read(mCpu->pc);
  mCpu->pc++;
  uint16_t ptrHigh = mCpu->Read(mCpu->pc);
  mCpu->pc++;

  uint16_t ptr = (ptrHigh << 8) | ptrLow;

  if (ptrLow == 0x00FF) {
    mCpu->addrAbs = (mCpu->Read(ptr & 0xFF00) << 8) | mCpu->Read(ptr + 0);
  } else {
    mCpu->addrAbs = (mCpu->Read(ptr + 1) << 8) | mCpu->Read(ptr + 0);
  }
  return 0;
}

std::string IND::Disassemble(uint32_t& current) {
  auto address = Read16BitAddress(current);
  return fmt::format("({:#06x}) {{IND}}", address);
}
