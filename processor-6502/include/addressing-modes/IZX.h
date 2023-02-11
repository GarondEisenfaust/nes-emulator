#pragma once
#include "IAddressingMode.h"

class IZX : public IAddressingMode {
 public:
  IZX(Processor6502* cpu);
  virtual bool operator()();
};