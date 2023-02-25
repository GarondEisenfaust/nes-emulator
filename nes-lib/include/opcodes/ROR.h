#pragma once
#include "IOpcode.h"

class ROR : public IOpcode {
 public:
  ROR(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
