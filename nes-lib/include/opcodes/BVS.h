#pragma once
#include "IOpcode.h"

class BVS : public IOpcode {
 public:
  BVS(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
