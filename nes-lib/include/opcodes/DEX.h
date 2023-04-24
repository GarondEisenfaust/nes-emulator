#pragma once
#include "IOpcode.h"

class DEX : public IOpcode {
 public:
  DEX(Cpu* cpu);
  virtual void operator()();
};
