#pragma once
#include "IController.h"

class Controller : public IController {
 public:
  void Write(uint16_t address, uint8_t data);
  uint8_t Read(uint16_t address);
};
