#pragma once
#include "IOpcode.h"

class ORA : public IOpcode {
 public:
  ORA(Processor6502* cpu);
  virtual bool operator()();
};
