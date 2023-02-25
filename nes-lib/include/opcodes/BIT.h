#pragma once
#include "IOpcode.h"

class BIT : public IOpcode {
 public:
  BIT(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
