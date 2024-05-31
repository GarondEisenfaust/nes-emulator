#pragma once
#include "IOpcode.h"

class CLD : public IOpcode {
 public:
  CLD(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
