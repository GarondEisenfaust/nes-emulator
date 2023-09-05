#include "Sequencer.h"

uint32_t Rotate(uint32_t sequence) { return ((sequence & 0x0001) << 7) | ((sequence & 0x00FE) >> 1); }

void Sequencer::Clock() {
  if (enabled) {
    timer--;
    if (timer == 0xFFFF) {
      timer = reload;
      sequence = Rotate(sequence);
      output = sequence & 0x01;
    }
  }
}
