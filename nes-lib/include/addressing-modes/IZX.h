#pragma once
#include "IAddressingMode.h"

class IZX : public IAddressingMode {
 public:
  IZX(Cpu* cpu);
  virtual bool operator()();
};