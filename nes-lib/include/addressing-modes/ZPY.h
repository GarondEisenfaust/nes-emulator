#pragma once
#include "IAddressingMode.h"

class ZPY : public IAddressingMode {
 public:
  ZPY(Cpu* cpu);
  virtual bool operator()();
};