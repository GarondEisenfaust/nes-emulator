#pragma once
#include "IOpcode.h"

class DEY : public IOpcode {
 public:
  DEY(Processor6502* cpu);
  virtual bool operator()();
};
