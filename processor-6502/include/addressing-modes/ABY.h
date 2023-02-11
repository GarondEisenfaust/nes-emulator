#pragma once
#include "IAddressingMode.h"

class ABY : public IAddressingMode {
 public:
  ABY(Processor6502* cpu);
  virtual bool operator()();
};