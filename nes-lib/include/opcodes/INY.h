#pragma once
#include "IOpcode.h"

class INY : public IOpcode {
 public:
  INY(Cpu* cpu);
  virtual bool operator()();
};
