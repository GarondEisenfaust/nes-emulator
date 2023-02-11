#pragma once
#include "IOpcode.h"

class JSR : public IOpcode {
 public:
  JSR(Processor6502* cpu);
  virtual bool operator()();
};
