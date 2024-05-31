#pragma once
#include "IOpcode.h"

class TXS : public IOpcode {
 public:
  TXS(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
