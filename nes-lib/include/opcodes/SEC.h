#pragma once
#include "IOpcode.h"

class SEC : public IOpcode {
 public:
  SEC(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
