#pragma once
#include "IOpcode.h"

class JMP : public IOpcode {
 public:
  JMP(Processor6502* cpu);
  virtual bool operator()();
};
