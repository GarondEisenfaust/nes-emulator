#pragma once
#include "IOpcode.h"

class CLC : public IOpcode {
 public:
  CLC(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
