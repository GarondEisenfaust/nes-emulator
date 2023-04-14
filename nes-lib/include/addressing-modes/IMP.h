#pragma once
#include "IAddressingMode.h"

class IMP : public IAddressingMode {
 public:
  IMP(Cpu* cpu);
  virtual bool operator()();
};