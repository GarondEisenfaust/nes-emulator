#pragma once
#include "IOpcode.h"

class SED : public IOpcode {
 public:
  SED(Cpu* cpu);
  virtual void operator()();
};
