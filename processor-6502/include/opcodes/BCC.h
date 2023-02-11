#pragma once
#include "IOpcode.h"

class BCC : public IOpcode {
 public:
  BCC(Processor6502* cpu);
  virtual bool operator()();
};
