#include "opcodes/CLI.h"
#include "Processor6502.h"

CLI::CLI(Processor6502* cpu) : IOpcode(cpu) {}

bool CLI::operator()() {
  mCpu->status.i = false;
  return false;
}
