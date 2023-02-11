#include "addressing-modes/ABS.h"
#include "Bus.h"
#include "Processor6502.h"
#include "fmt/format.h"

ABS::ABS(Processor6502* cpu) : IAddressingMode(cpu) {}

bool ABS::operator()() {
  uint16_t low = mCpu->Read(mCpu->pc);
  mCpu->pc++;
  uint16_t high = mCpu->Read(mCpu->pc);
  mCpu->pc++;

  mCpu->addrAbs = (high << 8) | low;

  return 0;
}

std::string ABS::Disassemble(uint32_t& current) {
  auto lo = mCpu->mBus->CpuRead(current, true);
  current++;
  auto hi = mCpu->mBus->CpuRead(current, true);
  current++;
  return fmt::format("{:#06x} {{ABS}}", static_cast<uint16_t>(hi << 8) | lo, 4);
}
