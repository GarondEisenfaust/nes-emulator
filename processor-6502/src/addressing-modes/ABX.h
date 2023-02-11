#include "IAddressingMode.h"

class ABX : IAddressingMode {
 public:
  ABX(Processor6502* cpu);
  virtual bool operator()();
};