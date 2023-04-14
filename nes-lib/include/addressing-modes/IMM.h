#pragma once
#include "IAddressingMode.h"

class IMM : public IAddressingMode {
 public:
  IMM(Cpu* cpu);
  virtual bool operator()();
};