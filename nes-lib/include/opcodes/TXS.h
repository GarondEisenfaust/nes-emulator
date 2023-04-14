#pragma once
#include "IOpcode.h"

class TXS : public IOpcode {
 public:
  TXS(Cpu* cpu);
  virtual bool operator()();
};
