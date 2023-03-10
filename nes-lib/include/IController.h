#pragma once
#include <cstdint>

class IController {
 public:
  virtual ~IController() = default;

  virtual void Write(uint16_t address) = 0;
  virtual uint8_t Read(uint16_t address) = 0;
};
