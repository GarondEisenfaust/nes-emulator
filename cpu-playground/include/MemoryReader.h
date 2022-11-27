#pragma once
#include "RAM.h"
#include <memory>
#include <sstream>

class MemoryReader {
 public:
  MemoryReader(const RAM& ram, uint16_t offset = 0, uint16_t lines = 16);
  ~MemoryReader() = default;
  void Display();
  std::unique_ptr<std::stringstream> CreateDisplayStream() const;

 private:
  const RAM& mRam;
  const uint16_t mOffset;
  const uint16_t mLines;
};
