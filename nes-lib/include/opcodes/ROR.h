#pragma once
#include "IOpcode.h"

class ROR : public IOpcode {
 public:
  ROR(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
