#pragma once
#include "IOpcode.h"

class INX : public IOpcode {
 public:
  INX(Processor6502* cpu);
  virtual bool operator()();
};
