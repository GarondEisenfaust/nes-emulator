#pragma once
#include "IOpcode.h"

class TAX : public IOpcode {
 public:
  TAX(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
