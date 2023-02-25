#pragma once
#include "IOpcode.h"

class ROL : public IOpcode {
 public:
  ROL(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
