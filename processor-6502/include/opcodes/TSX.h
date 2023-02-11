#pragma once
#include "IOpcode.h"

class TSX : public IOpcode {
 public:
  TSX(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
