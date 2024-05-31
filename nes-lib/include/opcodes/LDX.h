#pragma once
#include "IOpcode.h"

class LDX : public IOpcode {
 public:
  LDX(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
