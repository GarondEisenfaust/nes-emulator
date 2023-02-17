#include "addressing-modes/IMM.h"
#include "Bus.h"
#include "Processor6502.h"
#include "fmt/format.h"

IMM::IMM(Processor6502* cpu) : IAddressingMode(cpu) {}

bool IMM::operator()() {
  mCpu->addrAbs = mCpu->pc++;
  return 0;
}

std::string IMM::Disassemble(uint32_t& current) {
  auto value = mCpu->mBus->CpuRead(current, true);
  current++;
  return fmt::format("{:#04x} {{IMM}}", value);
}
