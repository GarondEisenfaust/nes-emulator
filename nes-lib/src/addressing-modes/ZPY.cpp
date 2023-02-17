#include "addressing-modes/ZPY.h"
#include "Bus.h"
#include "Processor6502.h"
#include "fmt/format.h"

ZPY::ZPY(Processor6502* cpu) : IAddressingMode(cpu) {}

bool ZPY::operator()() {
  mCpu->addrAbs = mCpu->Read(mCpu->pc) + mCpu->y;
  mCpu->pc++;
  mCpu->addrAbs &= 0x00FF;
  return 0;
}

std::string ZPY::Disassemble(uint32_t& current) {
  auto value = mCpu->mBus->CpuRead(current, true);
  current++;
  return fmt::format("{:#04x}, Y {{ZPY}} ", value);
}
