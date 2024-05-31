#pragma once
#include "IOpcode.h"

class BNE : public IOpcode {
 public:
  BNE(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
