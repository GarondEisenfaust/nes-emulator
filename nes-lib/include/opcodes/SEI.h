#pragma once
#include "IOpcode.h"

class SEI : public IOpcode {
 public:
  SEI(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
