#pragma once
#include "IOpcode.h"

class BCC : public IOpcode {
 public:
  BCC(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
