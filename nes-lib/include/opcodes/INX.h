#pragma once
#include "IOpcode.h"

class INX : public IOpcode {
 public:
  INX(Cpu* cpu);
  virtual void operator()();
};
