#pragma once
#include <cstdint>

#define CONTROLLER_START 0x4016
#define CONTROLLER_END 0x4017

class IController {
 public:
  virtual ~IController() = default;

  virtual void Write(uint16_t address, uint8_t data) = 0;
  virtual uint8_t Read(uint16_t address) = 0;
};
