#include "addressing-modes/IAddressingMode.h"
#include "Bus.h"
#include "Cpu.h"

IAddressingMode::IAddressingMode(Cpu* cpu) : mCpu(cpu) {}

uint16_t IAddressingMode::Read16BitAddress(uint32_t& address) {
  auto lo = mCpu->mBus->CpuRead(address, true);
  address++;
  auto hi = mCpu->mBus->CpuRead(address, true);
  address++;
  return static_cast<uint16_t>(hi << 8) | lo;
}
