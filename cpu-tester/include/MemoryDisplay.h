#pragma once
#include "RAM.h"
#include <memory>
#include <sstream>

class MemoryDisplay {
 public:
  MemoryDisplay(const RAM& ram);
  ~MemoryDisplay() = default;
  void Display();

 private:
  std::unique_ptr<std::stringstream> CreateDisplayStream(uint16_t offset = 0, uint16_t lines = 16);
  const RAM& mRam;
};
