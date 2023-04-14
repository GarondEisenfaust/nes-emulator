#pragma once
#include "IAddressingMode.h"

class REL : public IAddressingMode {
 public:
  REL(Cpu* cpu);
  virtual bool operator()();
};