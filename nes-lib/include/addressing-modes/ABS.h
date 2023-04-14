#pragma once
#include "IAddressingMode.h"

class ABS : public IAddressingMode {
 public:
  ABS(Cpu* cpu);
  virtual bool operator()();
};