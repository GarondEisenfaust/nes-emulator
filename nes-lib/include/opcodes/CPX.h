#pragma once
#include "IOpcode.h"

class CPX : public IOpcode {
 public:
  CPX(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
