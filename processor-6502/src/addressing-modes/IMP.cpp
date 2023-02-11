#include "addressing-modes/IMP.h"
#include "Bus.h"
#include "Processor6502.h"
#include "fmt/format.h"

IMP::IMP(Processor6502* cpu) : IAddressingMode(cpu) {}

bool IMP::operator()() {
  mCpu->fetched = mCpu->a;
  return 0;
}

std::string IMP::Disassemble(uint32_t& current) { return "{IMP}"; }
