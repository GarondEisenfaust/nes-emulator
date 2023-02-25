#include "addressing-modes/ABS.h"
#include "Bus.h"
#include "Cpu.h"
#include "fmt/format.h"

ABS::ABS(Cpu* cpu) : IAddressingMode(cpu) {}

bool ABS::operator()() {
  uint16_t low = mCpu->Read(mCpu->pc);
  mCpu->pc++;
  uint16_t high = mCpu->Read(mCpu->pc);
  mCpu->pc++;

  mCpu->addrAbs = (high << 8) | low;

  return 0;
}

std::string ABS::Disassemble(uint32_t& current) {
  auto address = Read16BitAddress(current);
  return fmt::format("{:#06x} {{ABS}}", address);
}
