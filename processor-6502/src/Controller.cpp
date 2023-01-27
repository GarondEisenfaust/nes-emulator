#include "Controller.h"
#include <GLFW/glfw3.h>

Controller::Controller(GLFWwindow* window) : mWindow(window) {}

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

void Controller::SetControllerBitBasedOnInput() {
  Reset();
  mControllerRegister[0].up = Pressed(GLFW_KEY_W);
  mControllerRegister[0].left = Pressed(GLFW_KEY_A);
  mControllerRegister[0].down = Pressed(GLFW_KEY_S);
  mControllerRegister[0].right = Pressed(GLFW_KEY_D);
  mControllerRegister[0].select = Pressed(GLFW_KEY_F);
  mControllerRegister[0].start = Pressed(GLFW_KEY_G);
  mControllerRegister[0].b = Pressed(GLFW_KEY_H);
  mControllerRegister[0].a = Pressed(GLFW_KEY_J);
}

bool Controller::Pressed(int keyToCheck) {
  auto currentState = glfwGetKey(mWindow, keyToCheck);
  return currentState == GLFW_PRESS || currentState == GLFW_REPEAT;
}
