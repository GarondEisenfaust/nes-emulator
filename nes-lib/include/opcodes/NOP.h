#pragma once
#include "IOpcode.h"

class NOP : public IOpcode {
 public:
  NOP(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
