#pragma once
#include "IOpcode.h"

class LSR : public IOpcode {
 public:
  LSR(Cpu* cpu);
  virtual bool operator()();
};
