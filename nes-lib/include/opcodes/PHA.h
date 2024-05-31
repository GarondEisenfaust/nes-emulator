#pragma once
#include "IOpcode.h"

class PHA : public IOpcode {
 public:
  PHA(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
