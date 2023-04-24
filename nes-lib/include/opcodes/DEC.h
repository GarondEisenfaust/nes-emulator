#pragma once
#include "IOpcode.h"

class DEC : public IOpcode {
 public:
  DEC(Cpu* cpu);
  virtual void operator()();
};
