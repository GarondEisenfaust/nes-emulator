#pragma once
#include "IOpcode.h"

class ORA : public IOpcode {
 public:
  ORA(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
