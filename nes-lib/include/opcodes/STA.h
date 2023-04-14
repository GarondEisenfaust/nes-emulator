#pragma once
#include "IOpcode.h"

class STA : public IOpcode {
 public:
  STA(Cpu* cpu);
  virtual bool operator()();
};
