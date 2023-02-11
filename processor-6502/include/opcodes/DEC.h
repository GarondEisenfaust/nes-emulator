#pragma once
#include "IOpcode.h"

class DEC : public IOpcode {
 public:
  DEC(Processor6502* cpu);
  virtual bool operator()();
};
