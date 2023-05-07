#pragma once
#include <cstdint>

class Sequencer {
 public:
  uint32_t sequence                  = 0x00000000;
  uint16_t timer                  = 0x0000;
  uint16_t reload                        = 0x0000;
  uint8_t output                   = 0x00;
  bool enabled                       = false;

  void Clock();
};
