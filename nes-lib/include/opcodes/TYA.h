#pragma once
#include "IOpcode.h"

class TYA : public IOpcode {
 public:
  TYA(Cpu* cpu);
  virtual void operator()();
};
