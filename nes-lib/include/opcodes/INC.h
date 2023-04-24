#pragma once
#include "IOpcode.h"

class INC : public IOpcode {
 public:
  INC(Cpu* cpu);
  virtual void operator()();
};
