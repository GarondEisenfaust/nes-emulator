#pragma once
#include "IOpcode.h"

class TXA : public IOpcode {
 public:
  TXA(Processor6502* cpu);
  virtual bool operator()();
};
