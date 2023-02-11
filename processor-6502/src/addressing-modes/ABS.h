#include "IAddressingMode.h"

class ABS : IAddressingMode {
 public:
  ABS(Processor6502* cpu);
  virtual bool operator()();
};