#include "IAddressingMode.h"

class ZP0 : IAddressingMode {
 public:
  ZP0(Processor6502* cpu);
  virtual bool operator()();
};