#pragma once
#include "IOpcode.h"

class BEQ : public IOpcode {
 public:
  BEQ(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
