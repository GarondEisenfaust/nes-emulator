#include "Button.h"
#include "ShapeRendering/SolidRectangleRenderer.h"

Button::Button(int x, int y, int width, int height, SolidRectangleRenderer& renderer)
    : mX(x),
      mY(y),
      mWidth(width),
      mHeight(height),
      mRectangle(renderer.CreateRectangle(x, y, width, height, {0.5, 0, 0, 1})) {}

void Button::UpdatePressedState(int x, int y) {
  if (!mIsPressed) {
    mIsPressed = (mX < x && x < (mX + mWidth)) && (mY < y && y < (mY + mHeight));
  }
  if (mIsPressed) {
    mRectangle.Highlight();
  }
}

void Button::ClearPressedState() {
  mIsPressed = false;
  mRectangle.DontHighlight();
}

bool Button::isPressed() { return mIsPressed; }
