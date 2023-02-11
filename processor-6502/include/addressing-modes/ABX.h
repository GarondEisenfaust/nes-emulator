#pragma once
#include "IAddressingMode.h"

class ABX : public IAddressingMode {
 public:
  ABX(Processor6502* cpu);
  virtual bool operator()();
};