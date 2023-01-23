#pragma once
#include <cstdint>

class Controller {
 public:
  void Write(uint16_t address);
  uint8_t Read(uint16_t address);

  void Reset();
  void SetControllerBitBasedOnInput(int key, int action);

 private:
  union ControllerRegister {
    struct {
      bool right : 1;
      bool left : 1;
      bool down : 1;
      bool up : 1;
      bool start : 1;
      bool select : 1;
      bool b : 1;
      bool a : 1;
    };
    uint8_t reg = 0x00;
  };

  uint8_t controllerBuffer[2];
  ControllerRegister mControllerRegister[2];
};
