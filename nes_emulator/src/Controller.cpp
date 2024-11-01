#include "Controller.h"

Controller::Controller(SolidRectangleRenderer& renderer)
    : mRenderer(renderer),
      mButtonLeft(25, 300, 200, 100, mRenderer),
      mButtonRight(325, 300, 200, 100, mRenderer),
      mButtonUp(225, 400, 100, 200, mRenderer),
      mButtonDown(225, 100, 100, 200, mRenderer),
      mButtonStart(1895, 800, 200, 100, mRenderer),
      mButtonSelect(125, 800, 200, 100, mRenderer),
      mButtonA(2000, 300, 200, 100, mRenderer),
      mButtonB(1750, 300, 200, 100, mRenderer) {}

void Controller::CheckButtons(int index, int x, int y) {
  mControllerRegister[index].a = mButtonA.isPressed(x, y) || mControllerRegister[index].a;
  mControllerRegister[index].b = mButtonB.isPressed(x, y) || mControllerRegister[index].b;
  mControllerRegister[index].start = mButtonStart.isPressed(x, y) || mControllerRegister[index].start;
  mControllerRegister[index].select = mButtonSelect.isPressed(x, y) || mControllerRegister[index].select;
  mControllerRegister[index].up = mButtonUp.isPressed(x, y) || mControllerRegister[index].up;
  mControllerRegister[index].down = mButtonDown.isPressed(x, y) || mControllerRegister[index].down;
  mControllerRegister[index].right = mButtonRight.isPressed(x, y) || mControllerRegister[index].right;
  mControllerRegister[index].left = mButtonLeft.isPressed(x, y) || mControllerRegister[index].left;
}

void Controller::Write(uint16_t address, uint8_t data) {
  const auto controllerIndex = data & 0x0001;
  controllerBuffer[controllerIndex] = mControllerRegister[controllerIndex].reg;
}

uint8_t Controller::Read(uint16_t address) {
  const auto controllerIndex = address & 0x0001;
  const uint8_t data = (controllerBuffer[controllerIndex] & (1 << 7)) > 0;
  controllerBuffer[controllerIndex] <<= 1;
  return data;
}

void Controller::ResetRegisters() {
  mControllerRegister[0].reg = 0x00;
  mControllerRegister[1].reg = 0x00;
}
