#pragma once
#include "IOpcode.h"

class SEI : public IOpcode {
 public:
  SEI(Cpu* cpu);
  virtual bool operator()();
};
