#pragma once
#include "IOpcode.h"

class CPY : public IOpcode {
 public:
  CPY(Cpu* cpu);
  virtual void operator()();
};
