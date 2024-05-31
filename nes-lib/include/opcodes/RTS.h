#pragma once
#include "IOpcode.h"

class RTS : public IOpcode {
 public:
  RTS(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
