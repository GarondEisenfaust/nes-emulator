#pragma once
#include "IOpcode.h"

class TAY : public IOpcode {
 public:
  TAY(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
