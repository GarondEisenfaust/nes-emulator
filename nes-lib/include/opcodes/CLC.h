#pragma once
#include "IOpcode.h"

class CLC : public IOpcode {
 public:
  CLC(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
