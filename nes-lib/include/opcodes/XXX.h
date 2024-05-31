#pragma once
#include "IOpcode.h"

class XXX : public IOpcode {
 public:
  XXX(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
