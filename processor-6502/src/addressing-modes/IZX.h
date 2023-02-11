#include "IAddressingMode.h"

class IZX : IAddressingMode {
 public:
  IZX(Processor6502* cpu);
  virtual bool operator()();
};