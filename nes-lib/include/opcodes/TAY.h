#pragma once
#include "IOpcode.h"

class TAY : public IOpcode {
 public:
  TAY(Cpu* cpu);
  virtual void operator()();
};
