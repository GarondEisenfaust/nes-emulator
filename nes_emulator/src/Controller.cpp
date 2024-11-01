#include "Controller.h"
#include "ShapeRendering/SolidRectangleRenderer.h"

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

void Controller::CheckIfButtonPressed(int index, const Touch& touch) { CheckIfButtonPressed(index, touch.x, touch.y); }

void Controller::CheckIfButtonPressed(int index, int x, int y) {
  mControllerRegister[index].a = mButtonA.isPressed(x, y) || mControllerRegister[index].a;
  mControllerRegister[index].b = mButtonB.isPressed(x, y) || mControllerRegister[index].b;
  mControllerRegister[index].start = mButtonStart.isPressed(x, y) || mControllerRegister[index].start;
  mControllerRegister[index].select = mButtonSelect.isPressed(x, y) || mControllerRegister[index].select;
  mControllerRegister[index].up = mButtonUp.isPressed(x, y) || mControllerRegister[index].up;
  mControllerRegister[index].down = mButtonDown.isPressed(x, y) || mControllerRegister[index].down;
  mControllerRegister[index].right = mButtonRight.isPressed(x, y) || mControllerRegister[index].right;
  mControllerRegister[index].left = mButtonLeft.isPressed(x, y) || mControllerRegister[index].left;
}

void Controller::CheckIfButtonsPressed(int index) {
  for (const auto& [id, touch] : inputEvents) {
    CheckIfButtonPressed(index, touch);
  }
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

void Controller::HandleInputEvents(android_input_buffer* inputBuffer) {
  CheckTouchEvents(inputBuffer);
  for (int i = 0; i < controllerBuffer.size(); i++) {
    ResetRegisters(i);
    CheckIfButtonsPressed(i);
  }
}

void Controller::CheckTouchEvents(android_input_buffer* inputBuffer) {
  for (size_t i = 0; i < inputBuffer->motionEventsCount; i++) {
    const auto* motionEvent = &inputBuffer->motionEvents[i];

    const int action = motionEvent->action;
    const int actionMasked = action & AMOTION_EVENT_ACTION_MASK;
    const int ptrIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

    const bool notPressed = actionMasked == AMOTION_EVENT_ACTION_UP ||
                            actionMasked == AMOTION_EVENT_ACTION_POINTER_UP ||
                            actionMasked == AMOTION_EVENT_ACTION_CANCEL;

    const auto* pointer = &motionEvent->pointers[ptrIndex];
    const int x = static_cast<int>(GameActivityPointerAxes_getX(pointer));
    const int y = mRenderer.GetScreenHeight() - static_cast<int>(GameActivityPointerAxes_getY(pointer));
    HandleEvent({pointer->id, notPressed ? UP : DOWN, x, y});
  }

  android_app_clear_motion_events(inputBuffer);
}

void Controller::HandleEvent(const Touch& touch) {
  if (touch.event == UP) {
    inputEvents.erase(touch.id);
    return;
  }
  inputEvents.insert({touch.id, touch});
}

void Controller::ResetRegisters(int index) { mControllerRegister[index].reg = 0x00; }
