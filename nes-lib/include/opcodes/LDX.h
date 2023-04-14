#pragma once
#include "IOpcode.h"

class LDX : public IOpcode {
 public:
  LDX(Cpu* cpu);
  virtual bool operator()();
};
