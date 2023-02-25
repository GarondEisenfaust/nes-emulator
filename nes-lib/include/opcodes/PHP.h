#pragma once
#include "IOpcode.h"

class PHP : public IOpcode {
 public:
  PHP(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
