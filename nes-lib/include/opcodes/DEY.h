#pragma once
#include "IOpcode.h"

class DEY : public IOpcode {
 public:
  DEY(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
