#pragma once
#include "IOpcode.h"

class SBC : public IOpcode {
 public:
  SBC(Cpu* cpu);
  virtual bool operator()();
};
