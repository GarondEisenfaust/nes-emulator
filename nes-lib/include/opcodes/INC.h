#pragma once
#include "IOpcode.h"

class INC : public IOpcode {
 public:
  INC(Cpu* cpu);
  virtual bool operator()();
};
