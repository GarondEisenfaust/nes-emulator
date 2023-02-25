#include "opcodes/ADC.h"
#include "Cpu.h"

ADC::ADC(Cpu* cpu) : IOpcode(cpu) {}

bool ADC::operator()() {
  mCpu->Fetch();
  auto temp = mCpu->a + mCpu->fetched + mCpu->status.c;
  mCpu->status.c = temp > 255;
  mCpu->status.z = (temp & 0x00FF) == 0;

  mCpu->status.v = (~(mCpu->a ^ mCpu->fetched) & (mCpu->a ^ temp)) & 0x0080;
  mCpu->status.n = temp & 0x80;
  mCpu->a = temp & 0x00FF;
  return 1;
}

const char* ADC::Name() { return "ADC"; }
