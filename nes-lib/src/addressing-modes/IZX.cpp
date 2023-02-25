#include "addressing-modes/IZX.h"
#include "Bus.h"
#include "Cpu.h"
#include "fmt/format.h"

IZX::IZX(Cpu* cpu) : IAddressingMode(cpu) {}

bool IZX::operator()() {
  uint16_t t = mCpu->Read(mCpu->pc);
  mCpu->pc++;

  uint16_t lo = mCpu->Read((t + mCpu->x) & 0x00FF);
  uint16_t hi = mCpu->Read((t + mCpu->x + 1) & 0x00FF);

  mCpu->addrAbs = (hi << 8) | lo;

  return 0;
}

std::string IZX::Disassemble(uint32_t& current) {
  auto value = mCpu->mBus->CpuRead(current, true);
  current++;
  return fmt::format("({:#04x}, X) {{IZX}}", value);
}
