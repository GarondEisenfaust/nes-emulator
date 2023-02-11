#include "IAddressingMode.h"

class IZY : IAddressingMode {
 public:
  IZY(Processor6502* cpu);
  virtual bool operator()();
};