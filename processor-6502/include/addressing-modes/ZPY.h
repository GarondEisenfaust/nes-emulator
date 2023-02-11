#pragma once
#include "IAddressingMode.h"

class ZPY : public IAddressingMode {
 public:
  ZPY(Processor6502* cpu);
  virtual bool operator()();
};