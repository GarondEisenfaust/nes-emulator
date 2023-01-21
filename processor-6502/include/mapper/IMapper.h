#pragma once
#include <cstdint>

#define MAPPER_START 0x8000
#define MAPPER_END 0xFFFF

struct MappingResult {
  uint32_t mappedAddress;
  bool mapped;
};

class IMapper {
 public:
  virtual ~IMapper() = default;

  virtual bool CpuMapRead(uint16_t address, uint32_t& mappedAddr) = 0;
  virtual bool CpuMapWrite(uint16_t address, uint32_t& mappedAddr, uint8_t data) = 0;

  virtual bool PpuMapRead(uint16_t address, uint32_t& mappedAddr) = 0;
  virtual bool PpuMapWrite(uint16_t address, uint32_t& mappedAddr) = 0;

  virtual void Reset() = 0;
};
