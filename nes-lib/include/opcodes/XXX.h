#pragma once
#include "IOpcode.h"

class XXX : public IOpcode {
 public:
  XXX(Cpu* cpu);
  virtual bool operator()();
};
