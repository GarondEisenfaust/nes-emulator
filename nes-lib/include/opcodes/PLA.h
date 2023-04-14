#pragma once
#include "IOpcode.h"

class PLA : public IOpcode {
 public:
  PLA(Cpu* cpu);
  virtual bool operator()();
};
