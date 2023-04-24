#pragma once
#include "IOpcode.h"

class AND : public IOpcode {
 public:
  AND(Cpu* cpu);
  virtual void operator()();
};
