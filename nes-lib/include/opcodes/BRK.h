#pragma once
#include "IOpcode.h"

class BRK : public IOpcode {
 public:
  BRK(Cpu* cpu);
  virtual void operator()();
};
