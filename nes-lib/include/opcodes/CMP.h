#pragma once
#include "IOpcode.h"

class CMP : public IOpcode {
 public:
  CMP(Cpu* cpu);
  virtual void operator()();
};
