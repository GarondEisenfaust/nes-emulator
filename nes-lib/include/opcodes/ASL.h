#pragma once
#include "IOpcode.h"

class ASL : public IOpcode {
 public:
  ASL(Cpu* cpu);
  virtual bool operator()();
};
