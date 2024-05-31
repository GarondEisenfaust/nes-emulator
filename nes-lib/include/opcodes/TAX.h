#pragma once
#include "IOpcode.h"

class TAX : public IOpcode {
 public:
  TAX(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
