#pragma once
#include "Processor6502.h"
#include "RAM.h"
#include <cstdint>
#include <memory>

class Bus {
 public:
  Bus();
  ~Bus();
  Processor6502 cpu;
  std::unique_ptr<RAM> ram;

  void Write(uint16_t addr, uint8_t data);
  uint8_t Read(uint16_t addr, bool bReadOnly = false);
};
