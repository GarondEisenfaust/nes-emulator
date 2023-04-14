#pragma once
#include "IOpcode.h"

class RTI : public IOpcode {
 public:
  RTI(Cpu* cpu);
  virtual bool operator()();
};
