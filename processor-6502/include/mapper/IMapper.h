#pragma once
#include <cstdint>

class IMapper {
 public:
  struct MappingResult {
    uint16_t mappedAddress;
    bool mapped;
  };

  virtual ~IMapper() = default;

 public:
  virtual MappingResult MapRead(uint16_t address) = 0;
  virtual MappingResult MapWrite(uint16_t address) = 0;
};
