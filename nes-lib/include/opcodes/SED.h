#pragma once
#include "IOpcode.h"

class SED : public IOpcode {
 public:
  SED(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
