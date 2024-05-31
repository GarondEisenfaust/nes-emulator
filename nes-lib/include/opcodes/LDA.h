#pragma once
#include "IOpcode.h"

class LDA : public IOpcode {
 public:
  LDA(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
