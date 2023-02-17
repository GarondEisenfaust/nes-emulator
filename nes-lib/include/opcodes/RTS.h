#pragma once
#include "IOpcode.h"

class RTS : public IOpcode {
 public:
  RTS(Processor6502* cpu);
  virtual bool operator()();
    virtual const char* Name();
};
