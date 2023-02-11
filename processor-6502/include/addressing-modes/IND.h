#pragma once
#include "IAddressingMode.h"

class IND : public IAddressingMode {
 public:
  IND(Processor6502* cpu);
  virtual bool operator()();
};