#include "Controller.h"
#include <GLFW/glfw3.h>

void Controller::Write(uint16_t address) {
  const auto controllerIndex = address & 0x0001;
  controllerBuffer[controllerIndex] = mControllerRegister[controllerIndex].reg;
}

uint8_t Controller::Read(uint16_t address) {
  const auto controllerIndex = address & 0x0001;
  uint8_t data = (controllerBuffer[controllerIndex] & 0x80) > 0;
  controllerBuffer[controllerIndex] <<= 1;
  return data;
}

void Controller::Reset() {
  mControllerRegister[0].reg = 0x00;
  mControllerRegister[1].reg = 0x00;
}

bool Pressed(int keyToCheck, int key, int action) {
  return key == keyToCheck && (action == GLFW_PRESS || action == GLFW_REPEAT);
}

void Controller::SetControllerBitBasedOnInput(int key, int action) {
  mControllerRegister[0].up = Pressed(GLFW_KEY_W, key, action);
  mControllerRegister[0].left = Pressed(GLFW_KEY_A, key, action);
  mControllerRegister[0].down = Pressed(GLFW_KEY_S, key, action);
  mControllerRegister[0].right = Pressed(GLFW_KEY_D, key, action);
  mControllerRegister[0].select = Pressed(GLFW_KEY_F, key, action);
  mControllerRegister[0].start = Pressed(GLFW_KEY_G, key, action);
  mControllerRegister[0].b = Pressed(GLFW_KEY_H, key, action);
  mControllerRegister[0].a = Pressed(GLFW_KEY_J, key, action);
}
