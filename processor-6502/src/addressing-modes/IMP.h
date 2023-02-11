#include "IAddressingMode.h"

class IMP : IAddressingMode {
 public:
  IMP(Processor6502* cpu);
  virtual bool operator()();
};