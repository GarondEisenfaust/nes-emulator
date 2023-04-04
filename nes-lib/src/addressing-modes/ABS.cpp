#include "addressing-modes/ABS.h"
#include "Bus.h"
#include "Cpu.h"
#include "fmt/format.h"

ABS::ABS(Cpu* cpu) : IAddressingMode(cpu) {}

bool ABS::operator()() {
  mCpu->addrAbs = mCpu->ReadTwoBytes(mCpu->pc);
  mCpu->pc += 2;
  return false;
}

std::string ABS::Disassemble(uint32_t& current) {
  auto address = mCpu->ReadTwoBytes(current);
  current += 2;
  return fmt::format("{:#06x} {{ABS}}", address);
}
