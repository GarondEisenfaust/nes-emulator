#include "Ram.h"

uint8_t Ram::Read(uint16_t addr) { return mRam[addr & RAM_SIZE]; }
void Ram::Write(uint16_t addr, uint8_t data) { mRam[addr & RAM_SIZE] = data; }
