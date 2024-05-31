#pragma once
#include "IOpcode.h"

class TSX : public IOpcode {
 public:
  TSX(Cpu* cpu);
  virtual void operator()();
  virtual const char* Name();
};
