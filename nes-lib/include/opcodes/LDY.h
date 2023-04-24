#pragma once
#include "IOpcode.h"

class LDY : public IOpcode {
 public:
  LDY(Cpu* cpu);
  virtual void operator()();
};
