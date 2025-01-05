#include "Controller.h"
#include "ShapeRendering/SolidRectangleRenderer.h"

Controller::Controller(SolidRectangleRenderer& renderer)
    : mRenderer(renderer),
      mScreenWidth(renderer.GetScreenWidth()),
      mScreenHeight(renderer.GetScreenHeight()),
      mButtonWidth(mScreenWidth * 0.09),
      mButtonHeight(mScreenHeight * 0.09),
      mButtonB(mScreenWidth * 0.78, mScreenHeight * 0.25, mButtonWidth, mButtonHeight, mRenderer),
      mButtonA(mScreenWidth * 0.89, mScreenHeight * 0.25, mButtonWidth, mButtonHeight, mRenderer),
      mButtonStart(mScreenWidth * 0.925 - mButtonWidth, mScreenHeight * 0.7, mButtonWidth, mButtonHeight, mRenderer),
      mButtonSelect(mScreenWidth * 0.075, mScreenHeight * 0.7, mButtonWidth, mButtonHeight, mRenderer),
      mDPad(mRenderer, mScreenWidth * 0.123, mScreenHeight * 0.3, mButtonWidth, mButtonHeight) {}

void Controller::UpdateButtonState(int index, const Touch& touch) { UpdateButtonState(index, touch.x, touch.y); }

void Controller::UpdateButtonState(int index, int x, int y) {
  mButtonA.UpdatePressedState(x, y);
  mButtonB.UpdatePressedState(x, y);
  mButtonStart.UpdatePressedState(x, y);
  mButtonSelect.UpdatePressedState(x, y);
  mDPad.UpdatePressedState(x, y);
}

void Controller::UpdateButtonState(int index) {
  for (const auto& [id, touch] : mInputEvents) {
    UpdateButtonState(index, touch);
  }
}

void Controller::Write(uint16_t address, uint8_t data) {
  const auto controllerIndex = data & 0x0001;
  mControllerBuffer[controllerIndex] = mControllerRegister[controllerIndex].reg;
}

uint8_t Controller::Read(uint16_t address) {
  const auto controllerIndex = address & 0x0001;
  const uint8_t data = (mControllerBuffer[controllerIndex] & (1 << 7)) > 0;
  mControllerBuffer[controllerIndex] <<= 1;
  return data;
}

void Controller::HandleInputEvents(android_input_buffer* inputBuffer) {
  CheckTouchEvents(inputBuffer);
  ClearPressedState();
  for (int i = 0; i < mControllerBuffer.size(); i++) {
    ResetRegisters(i);
    UpdateButtonState(i);
    UpdateControllerRegister(i);
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
    mInputEvents.erase(touch.id);
    return;
  }
  mInputEvents.insert({touch.id, touch});
}

void Controller::ResetRegisters(int index) { mControllerRegister[index].reg = 0x00; }

void Controller::UpdateControllerRegister(int index) {
  mControllerRegister[index].a = mButtonA.isPressed();
  mControllerRegister[index].b = mButtonB.isPressed();
  mControllerRegister[index].start = mButtonStart.isPressed();
  mControllerRegister[index].select = mButtonSelect.isPressed();
  mDPad.UpdateControllerRegister(mControllerRegister[index]);
}

void Controller::ClearPressedState() {
  mButtonA.ClearPressedState();
  mButtonB.ClearPressedState();
  mButtonStart.ClearPressedState();
  mButtonSelect.ClearPressedState();
  mDPad.ClearPressedState();
}

Controller::DPad::DPad(SolidRectangleRenderer& renderer, int positionX, int positionY, int buttonWidth,
                       int buttonHeight)
    : mPositionX(positionX),
      mPositionY(positionY),
      mButtonWidth(buttonWidth),
      mButtonHeight(buttonHeight),
      mMiddleSide(mButtonHeight / 2),
      mHalfDpadSize(mMiddleSide + mButtonWidth),
      mButtonLeft(mPositionX - mHalfDpadSize, mPositionY - mMiddleSide, mButtonWidth, mButtonHeight, renderer),
      mButtonRight(mPositionX + mMiddleSide, mPositionY - mMiddleSide, mButtonWidth, mButtonHeight, renderer),
      mButtonUp(mPositionX - mMiddleSide, mPositionY + mMiddleSide, mButtonHeight, mButtonWidth, renderer),
      mButtonDown(mPositionX - mMiddleSide, mPositionY - mHalfDpadSize, mButtonHeight, mButtonWidth, renderer) {}

void Controller::DPad::UpdatePressedState(int x, int y) {
  mButtonUp.UpdatePressedState(x, y);
  mButtonDown.UpdatePressedState(x, y);
  mButtonLeft.UpdatePressedState(x, y);
  mButtonRight.UpdatePressedState(x, y);
}

void Controller::DPad::UpdateControllerRegister(ControllerRegister& reg) {
  reg.up = mButtonUp.isPressed();
  reg.down = mButtonDown.isPressed();
  reg.left = mButtonLeft.isPressed();
  reg.right = mButtonRight.isPressed();
}

void Controller::DPad::ClearPressedState() {
  mButtonUp.ClearPressedState();
  mButtonDown.ClearPressedState();
  mButtonLeft.ClearPressedState();
  mButtonRight.ClearPressedState();
}
