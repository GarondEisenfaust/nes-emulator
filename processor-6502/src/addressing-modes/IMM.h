#include "IAddressingMode.h"

class IMM : IAddressingMode {
 public:
  IMM(Processor6502* cpu);
  virtual bool operator()();
};