#pragma once
#include <array>
#include <cstdint>

#define RAM_START 0x0000
#define RAM_END 0x1FFF
#define RAM_SIZE 0x07FF

class Ram {
 public:
  uint8_t Read(uint16_t addr);
  void Write(uint16_t addr, uint8_t data);

 private:
  std::array<uint8_t, RAM_SIZE + 1> mRam;
};
