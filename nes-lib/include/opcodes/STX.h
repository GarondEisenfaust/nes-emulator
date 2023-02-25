#pragma once
#include "IOpcode.h"

class STX : public IOpcode {
 public:
  STX(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
