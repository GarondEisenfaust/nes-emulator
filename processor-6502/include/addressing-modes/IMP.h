#pragma once
#include "IAddressingMode.h"

class IMP : public IAddressingMode {
 public:
  IMP(Processor6502* cpu);
  virtual bool operator()();
};