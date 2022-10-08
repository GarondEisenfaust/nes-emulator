#include "MemoryDisplay.h"
#include "Util.h"

MemoryDisplay::MemoryDisplay(const RAM& ram) : mRam(ram) {}

void MemoryDisplay::Display() {
  auto display = CreateDisplayStream()->str();
  std::cout << display << std::endl;

  display = CreateDisplayStream(0x8000)->str();
  std::cout << display << std::endl;
}

std::unique_ptr<std::stringstream> MemoryDisplay::CreateDisplayStream(uint16_t offset, uint16_t lines) {
  auto toDisplay = std::make_unique<std::stringstream>();

  for (uint currentLine = 0; currentLine < lines; currentLine++) {
    uint lineStartIndex = offset + currentLine * lines;
    uint lineEndIndex = lineStartIndex + lines;

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
