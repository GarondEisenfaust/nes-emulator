#include "Controller.h"

Controller::Controller(GLFWwindow* window) : mWindow(window) {}

void Controller::Write(uint16_t address) {
  const auto controllerIndex = address & 0x0001;
  controllerBuffer[controllerIndex] = mControllerRegister[controllerIndex].reg;
}

uint8_t Controller::Read(uint16_t address) {
  const auto controllerIndex = address & 0x0001;
  uint8_t data = (controllerBuffer[controllerIndex] & (1 << 7)) > 0;
  controllerBuffer[controllerIndex] <<= 1;
  return data;
}

void Controller::Reset() {
  mControllerRegister[0].reg = 0x00;
  mControllerRegister[1].reg = 0x00;
}

void Controller::SetControllerBitBasedOnInput() {
  Reset();
  glfwGetGamepadState(GLFW_JOYSTICK_1, &mGamepadState);

  mControllerRegister[0].up = PressedKeyboard(GLFW_KEY_W) || PressedGamepad(GLFW_GAMEPAD_BUTTON_DPAD_UP) ||
                              JoystickPositionBetween(GLFW_GAMEPAD_AXIS_LEFT_Y, -1, -0.4);
  mControllerRegister[0].left = PressedKeyboard(GLFW_KEY_A) || PressedGamepad(GLFW_GAMEPAD_BUTTON_DPAD_LEFT) ||
                                JoystickPositionBetween(GLFW_GAMEPAD_AXIS_LEFT_X, -1, -0.4);
  mControllerRegister[0].down = PressedKeyboard(GLFW_KEY_S) || PressedGamepad(GLFW_GAMEPAD_BUTTON_DPAD_DOWN) ||
                                JoystickPositionBetween(GLFW_GAMEPAD_AXIS_LEFT_Y, 0.4, 1);
  mControllerRegister[0].right = PressedKeyboard(GLFW_KEY_D) || PressedGamepad(GLFW_GAMEPAD_BUTTON_DPAD_RIGHT) ||
                                 JoystickPositionBetween(GLFW_GAMEPAD_AXIS_LEFT_X, 0.4, 1);
  mControllerRegister[0].select = PressedKeyboard(GLFW_KEY_F) || PressedGamepad(GLFW_GAMEPAD_BUTTON_BACK);
  mControllerRegister[0].start = PressedKeyboard(GLFW_KEY_G) || PressedGamepad(GLFW_GAMEPAD_BUTTON_START);
  mControllerRegister[0].b = PressedKeyboard(GLFW_KEY_H) || PressedGamepad(GLFW_GAMEPAD_BUTTON_X);
  mControllerRegister[0].a = PressedKeyboard(GLFW_KEY_J) || PressedGamepad(GLFW_GAMEPAD_BUTTON_A);
}

bool Controller::PressedKeyboard(int keyToCheck) {
  auto currentState = glfwGetKey(mWindow, keyToCheck);
  return currentState == GLFW_PRESS || currentState == GLFW_REPEAT;
}

bool Controller::PressedGamepad(int buttonToCheck) { return mGamepadState.buttons[buttonToCheck] == GLFW_PRESS; }

bool Controller::JoystickPositionBetween(int joystickAxis, float lower, float upper) {
  auto position = mGamepadState.axes[joystickAxis];
  return lower <= position && position <= upper;
}
