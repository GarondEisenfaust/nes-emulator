#include "MemoryReader.h"
#include "Util.h"

MemoryReader::MemoryReader(const RAM& ram, uint16_t offset, uint16_t lines)
    : mRam(ram), mOffset(offset), mLines(lines) {}

std::unique_ptr<std::stringstream> MemoryReader::CreateDisplayStream() const {
  auto toDisplay = std::make_unique<std::stringstream>();

  for (uint currentLine = 0; currentLine < mLines; currentLine++) {
    uint lineStartIndex = mOffset + currentLine * mLines;
    uint lineEndIndex = lineStartIndex + mLines;

    *toDisplay << "$" << Util::IntToHex(lineStartIndex).str() << ":  ";

    for (uint i = lineStartIndex; i < lineEndIndex; i++) {
      auto currValue = mRam[i];
      *toDisplay << Util::IntToHex(currValue).str();
      *toDisplay << " ";
    }
    *toDisplay << "\n";
  }
  auto bla = toDisplay->str();
  return std::move(toDisplay);
}
