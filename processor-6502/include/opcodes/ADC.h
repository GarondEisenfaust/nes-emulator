#pragma once
#include "IOpcode.h"

class ADC : public IOpcode {
 public:
  ADC(Processor6502* cpu);
  virtual bool operator()();
};
