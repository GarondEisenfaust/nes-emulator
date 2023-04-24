#pragma once
#include "IOpcode.h"

class BCS : public IOpcode {
 public:
  BCS(Cpu* cpu);
  virtual void operator()();
};
