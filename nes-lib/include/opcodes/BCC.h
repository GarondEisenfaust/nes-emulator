#pragma once
#include "IOpcode.h"

class BCC : public IOpcode {
 public:
  BCC(Cpu* cpu);
  virtual bool operator()();
};
