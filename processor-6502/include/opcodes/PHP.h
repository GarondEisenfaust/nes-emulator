#pragma once
#include "IOpcode.h"

class PHP : public IOpcode {
 public:
  PHP(Processor6502* cpu);
  virtual bool operator()();
};
