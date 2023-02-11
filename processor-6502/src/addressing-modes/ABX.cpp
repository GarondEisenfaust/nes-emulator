#include "addressing-modes/ABX.h"
#include "Bus.h"
#include "Processor6502.h"
#include "fmt/format.h"

ABX::ABX(Processor6502* cpu) : IAddressingMode(cpu) {}

bool ABX::operator()() {
  uint16_t low = mCpu->Read(mCpu->pc);
  mCpu->pc++;
  uint16_t high = mCpu->Read(mCpu->pc);
  mCpu->pc++;

  mCpu->addrAbs = ((high << 8) | low) + mCpu->x;

  if ((mCpu->addrAbs & 0xFF00) != (high << 8)) {
    return 1;
  } else {
    return 0;
  }
}

std::string ABX::Disassemble(uint32_t& current) {
  auto lo = mCpu->mBus->CpuRead(current, true);
  current++;
  auto hi = mCpu->mBus->CpuRead(current, true);
  current++;
  return fmt::format("{:#06x}, X {{ABX}}", static_cast<uint16_t>(hi << 8) | lo, 4);
}
