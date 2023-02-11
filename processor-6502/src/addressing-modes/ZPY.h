#include "IAddressingMode.h"

class ZPY : IAddressingMode {
 public:
  ZPY(Processor6502* cpu);
  virtual bool operator()();
};