#pragma once
#include "IOpcode.h"

class LSR : public IOpcode {
 public:
  LSR(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
