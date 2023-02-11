#include "addressing-modes/ZP0.h"
#include "Bus.h"
#include "Processor6502.h"
#include "fmt/format.h"

ZP0::ZP0(Processor6502* cpu) : IAddressingMode(cpu) {}

bool ZP0::operator()() {
  mCpu->addrAbs = mCpu->Read(mCpu->pc);
  mCpu->pc++;
  mCpu->addrAbs &= 0x00FF;
  return 0;
}

std::string ZP0::Disassemble(uint32_t& current) {
  auto value = mCpu->mBus->CpuRead(current, true);
  current++;
  return fmt::format("{:#04x} {{ZP0}}", value);
}
