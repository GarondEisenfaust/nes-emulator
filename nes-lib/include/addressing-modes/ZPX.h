#pragma once
#include "IAddressingMode.h"

class ZPX : public IAddressingMode {
 public:
  ZPX(Cpu* cpu);
  virtual bool operator()();
};