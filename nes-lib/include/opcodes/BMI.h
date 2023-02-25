#pragma once
#include "IOpcode.h"

class BMI : public IOpcode {
 public:
  BMI(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
