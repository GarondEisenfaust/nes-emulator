#pragma once
#include "IOpcode.h"

class SEC : public IOpcode {
 public:
  SEC(Cpu* cpu);
  virtual bool operator()();
};
