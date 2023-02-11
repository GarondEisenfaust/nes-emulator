#include "IAddressingMode.h"

class IND : IAddressingMode {
 public:
  IND(Processor6502* cpu);
  virtual bool operator()();
};