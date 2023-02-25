#pragma once
#include "IOpcode.h"

class CLD : public IOpcode {
 public:
  CLD(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
