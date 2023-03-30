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

  virtual uint32_t CpuMapRead(uint16_t address) = 0;
  virtual uint32_t CpuMapWrite(uint16_t address) = 0;

  virtual uint32_t PpuMapRead(uint16_t address) = 0;
  virtual uint32_t PpuMapWrite(uint16_t address) = 0;

  virtual void Reset() = 0;
};
