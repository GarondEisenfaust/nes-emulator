#pragma once
#include "IOpcode.h"

class ASL : public IOpcode {
 public:
  ASL(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
