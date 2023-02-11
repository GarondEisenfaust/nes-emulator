#pragma once
#include "IOpcode.h"

class BMI : public IOpcode {
 public:
  BMI(Processor6502* cpu);
  virtual bool operator()();
};
