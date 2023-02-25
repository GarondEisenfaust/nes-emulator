#pragma once
#include "IOpcode.h"

class BVC : public IOpcode {
 public:
  BVC(Cpu* cpu);
  virtual bool operator()();
  virtual const char* Name();
};
