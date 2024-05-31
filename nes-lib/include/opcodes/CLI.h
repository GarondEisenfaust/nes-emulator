#pragma once
#include "IOpcode.h"

class CLI : public IOpcode {
 public:
  CLI(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
