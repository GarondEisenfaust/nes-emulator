#pragma once
#include "IOpcode.h"

class PLP : public IOpcode {
 public:
  PLP(Cpu* cpu);
  virtual void operator()();
};
