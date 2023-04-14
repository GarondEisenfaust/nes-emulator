#pragma once
#include "IAddressingMode.h"

class IZY : public IAddressingMode {
 public:
  IZY(Cpu* cpu);
  virtual bool operator()();
};