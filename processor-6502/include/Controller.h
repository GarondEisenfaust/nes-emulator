#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <cstdint>

class GLFWwindow;
class Controller {
 public:
  Controller(GLFWwindow* window);
  void Write(uint16_t address);
  uint8_t Read(uint16_t address);

  void Reset();
  void SetControllerBitBasedOnInput();

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

  GLFWwindow* mWindow;
  uint8_t controllerBuffer[2];
  ControllerRegister mControllerRegister[2];

  GLFWgamepadstate mGamepadState;

  bool PressedKeyboard(int keyToCheck);
  bool PressedGamepad(int buttonToCheck);
  bool JoystickPositionBetween(int buttonToCheck, float lower, float upper);
};
