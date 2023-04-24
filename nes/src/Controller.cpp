#include "Controller.h"

Controller::Controller(GLFWwindow* window) : mWindow(window) {}

void Controller::Write(uint16_t address, uint8_t data) {
  const auto controllerIndex = data & 0x0001;
  controllerBuffer[controllerIndex] = mControllerRegister[controllerIndex].reg;
}

uint8_t Controller::Read(uint16_t address) {
  const auto controllerIndex = address & 0x0001;
  uint8_t data = (controllerBuffer[controllerIndex] & (1 << 7)) > 0;
  controllerBuffer[controllerIndex] <<= 1;
  return data;
}

void Controller::Reset(int gamepadIndex) { mControllerRegister[gamepadIndex].reg = 0x00; }

void Controller::SetControllerBitBasedOnInput(int gamepadIndex) {
  Reset(gamepadIndex);
  glfwGetGamepadState(gamepadIndex, &mGamepadState[gamepadIndex]);

  mControllerRegister[gamepadIndex].up = PressedKeyboard(GLFW_KEY_W) ||
                                         PressedGamepad(gamepadIndex, GLFW_GAMEPAD_BUTTON_DPAD_UP) ||
                                         JoystickPositionBetween(gamepadIndex, GLFW_GAMEPAD_AXIS_LEFT_Y, -1, -0.4);
  mControllerRegister[gamepadIndex].left = PressedKeyboard(GLFW_KEY_A) ||
                                           PressedGamepad(gamepadIndex, GLFW_GAMEPAD_BUTTON_DPAD_LEFT) ||
                                           JoystickPositionBetween(gamepadIndex, GLFW_GAMEPAD_AXIS_LEFT_X, -1, -0.4);
  mControllerRegister[gamepadIndex].down = PressedKeyboard(GLFW_KEY_S) ||
                                           PressedGamepad(gamepadIndex, GLFW_GAMEPAD_BUTTON_DPAD_DOWN) ||
                                           JoystickPositionBetween(gamepadIndex, GLFW_GAMEPAD_AXIS_LEFT_Y, 0.4, 1);
  mControllerRegister[gamepadIndex].right = PressedKeyboard(GLFW_KEY_D) ||
                                            PressedGamepad(gamepadIndex, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT) ||
                                            JoystickPositionBetween(gamepadIndex, GLFW_GAMEPAD_AXIS_LEFT_X, 0.4, 1);
  mControllerRegister[gamepadIndex].select =
      PressedKeyboard(GLFW_KEY_F) || PressedGamepad(gamepadIndex, GLFW_GAMEPAD_BUTTON_BACK);
  mControllerRegister[gamepadIndex].start =
      PressedKeyboard(GLFW_KEY_G) || PressedGamepad(gamepadIndex, GLFW_GAMEPAD_BUTTON_START);
  mControllerRegister[gamepadIndex].b =
      PressedKeyboard(GLFW_KEY_H) || PressedGamepad(gamepadIndex, GLFW_GAMEPAD_BUTTON_X);
  mControllerRegister[gamepadIndex].a =
      PressedKeyboard(GLFW_KEY_J) || PressedGamepad(gamepadIndex, GLFW_GAMEPAD_BUTTON_A);
}

bool Controller::PressedKeyboard(int keyToCheck) {
  auto currentState = glfwGetKey(mWindow, keyToCheck);
  return currentState == GLFW_PRESS || currentState == GLFW_REPEAT;
}

bool Controller::PressedGamepad(int gamepadIndex, int buttonToCheck) {
  return mGamepadState[gamepadIndex].buttons[buttonToCheck] == GLFW_PRESS;
}

bool Controller::JoystickPositionBetween(int gamepadIndex, int joystickAxis, float lower, float upper) {
  auto position = mGamepadState[gamepadIndex].axes[joystickAxis];
  return lower <= position && position <= upper;
}
