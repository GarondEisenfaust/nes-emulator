#pragma once
#include "IOpcode.h"

class CLV : public IOpcode {
 public:
  CLV(Processor6502* cpu);
  virtual bool operator()();
};
