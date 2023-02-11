#pragma once
#include "IAddressingMode.h"

class ABS : public IAddressingMode {
 public:
  ABS(Processor6502* cpu);
  virtual bool operator()();
};