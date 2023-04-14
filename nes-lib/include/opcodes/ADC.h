#pragma once
#include "IOpcode.h"

class ADC : public IOpcode {
 public:
  ADC(Cpu* cpu);
  virtual bool operator()();
};
