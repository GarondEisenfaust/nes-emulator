#pragma once
#include "IOpcode.h"

class ORA : public IOpcode {
 public:
  ORA(Cpu* cpu);
  virtual bool operator()();
};
