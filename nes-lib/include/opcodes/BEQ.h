#pragma once
#include "IOpcode.h"

class BEQ : public IOpcode {
 public:
  BEQ(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
