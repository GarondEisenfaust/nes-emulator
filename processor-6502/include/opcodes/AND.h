#pragma once
#include "IOpcode.h"

class AND : public IOpcode {
 public:
  AND(Processor6502* cpu);
  virtual bool operator()();
};
