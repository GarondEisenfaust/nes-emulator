#pragma once
#include "IAddressingMode.h"

class ABX : public IAddressingMode {
 public:
  ABX(Cpu* cpu);
  virtual bool operator()();
};