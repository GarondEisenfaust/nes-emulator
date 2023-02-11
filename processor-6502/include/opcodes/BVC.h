#pragma once
#include "IOpcode.h"

class BVC : public IOpcode {
 public:
  BVC(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
