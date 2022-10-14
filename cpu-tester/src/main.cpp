#include "Bus.h"
#include "GuiContext.h"
#include "Header.h"
#include "MemoryReader.h"
#include "Processor6502.h"
#include "windows/GuiWindow.h"
#include "windows/MemoryWindow.h"
#include "windows/RegisterWindow.h"
#include <iostream>
#include <memory>
#include <vector>

int main() {
  Bus bus;
  bus.ram->at(0) = 0x55;
  bus.ram->at(1) = 0x56;
  bus.ram->at(0x8000) = 0x77;

  Processor6502 cpu;
  cpu.ConnectBus(&bus);

  MemoryReader memoryReader1(*bus.ram);
  MemoryReader memoryReader2(*bus.ram, 0x8000);

  GuiContext guiContext;

  MemoryWindow memoryWindow1("Memory from 0 to 0x00f0", memoryReader1);
  MemoryWindow memoryWindow2("Memory from 0x8000 to 0x80f0", memoryReader2);
  RegisterWindow registerWindow("Registers", cpu);

  guiContext.mClientWindows.push_back(&memoryWindow1);
  guiContext.mClientWindows.push_back(&memoryWindow2);
  guiContext.mClientWindows.push_back(&registerWindow);
  guiContext.Loop();

  return 0;
}
