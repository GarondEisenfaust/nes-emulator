#pragma once
#include "IOpcode.h"

class BNE : public IOpcode {
 public:
  BNE(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
