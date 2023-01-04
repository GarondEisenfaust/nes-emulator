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

  virtual MappingResult CpuMapRead(uint16_t address) = 0;
  virtual MappingResult CpuMapWrite(uint16_t address) = 0;

  virtual MappingResult PpuMapRead(uint16_t address) = 0;
  virtual MappingResult PpuMapWrite(uint16_t address) = 0;

  virtual void Reset() = 0;
};
