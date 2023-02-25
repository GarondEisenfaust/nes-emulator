#pragma once
#include "IOpcode.h"

class INX : public IOpcode {
 public:
  INX(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
