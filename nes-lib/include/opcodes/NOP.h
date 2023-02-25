#pragma once
#include "IOpcode.h"

class NOP : public IOpcode {
 public:
  NOP(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
