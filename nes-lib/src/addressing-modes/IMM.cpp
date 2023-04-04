#include "addressing-modes/IMM.h"
#include "Bus.h"
#include "Cpu.h"
#include "fmt/format.h"

IMM::IMM(Cpu* cpu) : IAddressingMode(cpu) {}

bool IMM::operator()() {
  mCpu->addrAbs = mCpu->pc++;
  return false;
}

std::string IMM::Disassemble(uint32_t& current) {
  auto value = mCpu->mBus->CpuRead(current, true);
  current++;
  return fmt::format("{:#04x} {{IMM}}", value);
}
