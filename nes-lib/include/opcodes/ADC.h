#pragma once
#include "IOpcode.h"

class ADC : public IOpcode {
 public:
  ADC(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
