#pragma once
#include "IOpcode.h"

class PHP : public IOpcode {
 public:
  PHP(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
