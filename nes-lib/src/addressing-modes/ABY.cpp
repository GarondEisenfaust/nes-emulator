#include "addressing-modes/ABY.h"
#include "Bus.h"
#include "Cpu.h"
#include "fmt/format.h"

ABY::ABY(Cpu* cpu) : IAddressingMode(cpu) {}

bool ABY::operator()() {
  uint16_t low = mCpu->Read(mCpu->pc);
  mCpu->pc++;
  uint16_t high = mCpu->Read(mCpu->pc);
  mCpu->pc++;

  mCpu->addrAbs = ((high << 8) | low) + mCpu->y;

  if ((mCpu->addrAbs & 0xFF00) != (high << 8)) {
    return 1;
  } else {
    return 0;
  }
}

std::string ABY::Disassemble(uint32_t& current) {
  auto address = Read16BitAddress(current);
  return fmt::format("{:#06x}, Y {{ABY}}", address);
}
