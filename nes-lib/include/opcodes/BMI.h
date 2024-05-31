#pragma once
#include "IOpcode.h"

class BMI : public IOpcode {
 public:
  BMI(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
