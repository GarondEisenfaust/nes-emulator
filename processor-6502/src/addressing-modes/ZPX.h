#include "IAddressingMode.h"

class ZPX : IAddressingMode {
 public:
  ZPX(Processor6502* cpu);
  virtual bool operator()();
};