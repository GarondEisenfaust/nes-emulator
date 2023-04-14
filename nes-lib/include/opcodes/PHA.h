#pragma once
#include "IOpcode.h"

class PHA : public IOpcode {
 public:
  PHA(Cpu* cpu);
  virtual bool operator()();
};
