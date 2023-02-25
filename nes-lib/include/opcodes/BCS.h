#pragma once
#include "IOpcode.h"

class BCS : public IOpcode {
 public:
  BCS(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
