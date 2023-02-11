#pragma once
#include "IAddressingMode.h"

class IZY : public IAddressingMode {
 public:
  IZY(Processor6502* cpu);
  virtual bool operator()();
};