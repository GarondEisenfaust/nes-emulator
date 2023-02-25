#pragma once
#include "IOpcode.h"

class DEX : public IOpcode {
 public:
  DEX(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
