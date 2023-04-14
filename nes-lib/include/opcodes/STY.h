#pragma once
#include "IOpcode.h"

class STY : public IOpcode {
 public:
  STY(Cpu* cpu);
  virtual bool operator()();
};
