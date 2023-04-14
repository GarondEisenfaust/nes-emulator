#include "opcodes/CLI.h"
#include "Cpu.h"

CLI::CLI(Cpu* cpu) : IOpcode(cpu) {}

bool CLI::operator()() {
  mCpu->status.i = false;
  return false;
}
