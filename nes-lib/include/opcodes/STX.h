#pragma once
#include "IOpcode.h"

class STX : public IOpcode {
 public:
  STX(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
