#pragma once
#include "IOpcode.h"

class CLV : public IOpcode {
 public:
  CLV(Cpu* cpu);
  virtual void operator()();
};
