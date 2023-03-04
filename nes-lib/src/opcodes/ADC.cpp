#include "opcodes/ADC.h"
#include "Cpu.h"

ADC::ADC(Cpu* cpu) : IOpcode(cpu) {}

bool ADC::operator()() {
  mCpu->Fetch();
  auto temp = mCpu->a + mCpu->fetched + mCpu->status.c;
  mCpu->status.c = temp > 0xFF;
  mCpu->status.z = (temp & 0x00FF) == 0;

  mCpu->status.v = (~(mCpu->a ^ mCpu->fetched) & (mCpu->a ^ temp)) & (1 << 7);
  mCpu->status.n = temp & (1 << 7);
  mCpu->a = temp & 0x00FF;
  return 1;
}

const char* ADC::Name() { return "ADC"; }
