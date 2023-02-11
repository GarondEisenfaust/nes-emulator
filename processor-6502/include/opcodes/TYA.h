#pragma once
#include "IOpcode.h"

class TYA : public IOpcode {
 public:
  TYA(Processor6502* cpu);
  virtual bool operator()();
};
