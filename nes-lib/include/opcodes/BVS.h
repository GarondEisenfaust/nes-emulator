#pragma once
#include "IOpcode.h"

class BVS : public IOpcode {
 public:
  BVS(Cpu* cpu);
  virtual bool operator()();
};
