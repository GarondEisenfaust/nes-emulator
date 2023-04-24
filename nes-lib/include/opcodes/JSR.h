#pragma once
#include "IOpcode.h"

class JSR : public IOpcode {
 public:
  JSR(Cpu* cpu);
  virtual void operator()();
};
