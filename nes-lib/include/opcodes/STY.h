#pragma once
#include "IOpcode.h"

class STY : public IOpcode {
 public:
  STY(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
