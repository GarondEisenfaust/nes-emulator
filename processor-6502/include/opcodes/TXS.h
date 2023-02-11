#pragma once
#include "IOpcode.h"

class TXS : public IOpcode {
 public:
  TXS(Processor6502* cpu);
  virtual bool operator()();
};
