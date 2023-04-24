#pragma once
#include "IOpcode.h"

class JMP : public IOpcode {
 public:
  JMP(Cpu* cpu);
  virtual void operator()();
};
