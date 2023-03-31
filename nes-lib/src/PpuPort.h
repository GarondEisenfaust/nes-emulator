#pragma once

#define PPU_NUM_PORTS 8
enum PpuPort {
  Control = 0,
  Mask = 1,
  Status = 2,
  OamAddress = 3,
  OamData = 4,
  Scroll = 5,
  PpuAddress = 6,
  PpuData = 7
};
