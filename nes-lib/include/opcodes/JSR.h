#pragma once
#include "IOpcode.h"

class JSR : public IOpcode {
 public:
  JSR(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
