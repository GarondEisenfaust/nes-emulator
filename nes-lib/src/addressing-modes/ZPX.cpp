#include "addressing-modes/ZPX.h"
#include "Bus.h"
#include "Cpu.h"
#include "fmt/format.h"

ZPX::ZPX(Cpu* cpu) : IAddressingMode(cpu) {}

bool ZPX::operator()() {
  mCpu->addrAbs = mCpu->Read(mCpu->pc) + mCpu->x;
  mCpu->pc++;
  mCpu->addrAbs &= 0x00FF;
  return 0;
}

std::string ZPX::Disassemble(uint32_t& current) {
  auto value = mCpu->mBus->CpuRead(current, true);
  current++;
  return fmt::format("{:#04x}, X {{ZPX}}", value);
}
