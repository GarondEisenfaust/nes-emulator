#pragma once
#include "IOpcode.h"

class TXA : public IOpcode {
 public:
  TXA(Cpu* cpu);
  virtual void operator()();
};
