#pragma once
#include "IOpcode.h"

class ROL : public IOpcode {
 public:
  ROL(Cpu* cpu);
  virtual void operator()();
};
