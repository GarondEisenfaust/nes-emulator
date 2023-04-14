#pragma once
#include "IAddressingMode.h"

class ABY : public IAddressingMode {
 public:
  ABY(Cpu* cpu);
  virtual bool operator()();
};