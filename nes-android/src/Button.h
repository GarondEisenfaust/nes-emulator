#pragma once

class SolidRectangle;
class SolidRectangleRenderer;

class Button {
 public:
  Button(int x, int y, int width, int height, SolidRectangleRenderer& renderer);

  ~Button() = default;
  void UpdatePressedState(int x, int y);
  void ClearPressedState();
  bool isPressed();

 private:
  int mX;
  int mY;
  int mWidth;
  int mHeight;
  bool mIsPressed;
  SolidRectangle& mRectangle;
};
