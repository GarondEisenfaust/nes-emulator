#include "Bus.h"
#include "Header.h"
#include "MemoryDisplay.h"
#include <iostream>

int main() {
  Bus bus;
  bus.ram->at(0) = 0x55;
  bus.ram->at(1) = 0x56;

  MemoryDisplay memoryDisplay(*bus.ram);
  memoryDisplay.Display();
}
