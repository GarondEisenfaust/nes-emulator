#pragma once
#include "IOpcode.h"

class TSX : public IOpcode {
 public:
  TSX(Cpu* cpu);
  virtual bool operator()();
};
