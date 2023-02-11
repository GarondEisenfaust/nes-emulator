#include "IAddressingMode.h"

class REL : IAddressingMode {
 public:
  REL(Processor6502* cpu);
  virtual bool operator()();
};