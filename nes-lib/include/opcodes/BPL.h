#pragma once
#include "IOpcode.h"

class BPL : public IOpcode {
 public:
  BPL(Cpu* cpu);
  virtual bool operator()();
};
