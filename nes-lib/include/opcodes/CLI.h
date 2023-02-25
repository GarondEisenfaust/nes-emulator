#pragma once
#include "IOpcode.h"

class CLI : public IOpcode {
 public:
  CLI(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
