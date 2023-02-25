#pragma once
#include "IOpcode.h"

class EOR : public IOpcode {
 public:
  EOR(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
