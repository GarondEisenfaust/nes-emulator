#include "Bus.h"
#include "GuiContext.h"
#include "Header.h"
#include "MemoryReader.h"
#include "windows/GuiWindow.h"
#include "windows/MemoryWindow.h"
#include <iostream>
#include <memory>
#include <vector>

int main() {
  Bus bus;
  bus.ram->at(0) = 0x55;
  bus.ram->at(1) = 0x56;
  bus.ram->at(0x8000) = 0x77;

  MemoryReader memoryReader1(*bus.ram);
  MemoryReader memoryReader2(*bus.ram, 0x8000);

  GuiContext guiContext;

  guiContext.mClientWindows.push_back(std::make_unique<MemoryWindow>("Memory from 0 to 0x00f0", memoryReader1));
  guiContext.mClientWindows.push_back(std::make_unique<MemoryWindow>("Memory from 0x8000 to 0x80f0", memoryReader2));

  guiContext.Loop();

  return 0;
}
