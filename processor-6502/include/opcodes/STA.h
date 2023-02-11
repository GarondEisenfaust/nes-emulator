#pragma once
#include "IOpcode.h"

class STA : public IOpcode {
 public:
  STA(Processor6502* cpu);
  virtual bool operator()();
};
