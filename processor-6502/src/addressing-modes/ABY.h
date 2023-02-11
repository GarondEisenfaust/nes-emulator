#include "IAddressingMode.h"

class ABY : IAddressingMode {
 public:
  ABY(Processor6502* cpu);
  virtual bool operator()();
};