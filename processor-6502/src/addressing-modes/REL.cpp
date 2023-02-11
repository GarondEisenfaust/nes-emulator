#include "addressing-modes/REL.h"
#include "Bus.h"
#include "Processor6502.h"
#include "fmt/format.h"

REL::REL(Processor6502* cpu) : IAddressingMode(cpu) {}

bool REL::operator()() {
  mCpu->addrRel = mCpu->Read(mCpu->pc);
  mCpu->pc++;
  if (mCpu->addrRel & 0x80) {
    mCpu->addrRel |= 0xFF00;
  }
  return 0;
}

std::string REL::Disassemble(uint32_t& current) {
  auto value = mCpu->mBus->CpuRead(current, true);
  current++;
  return fmt::format("{:#04x} [{:#06x}] {{REL}}", value, current + static_cast<int8_t>(value));
}
