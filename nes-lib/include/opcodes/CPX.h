#pragma once
#include "IOpcode.h"

class CPX : public IOpcode {
 public:
  CPX(Cpu* cpu);
  virtual bool operator()();
};
