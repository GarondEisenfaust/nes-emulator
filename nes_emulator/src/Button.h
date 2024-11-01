#pragma once

class SolidRectangle;
class SolidRectangleRenderer;

class Button {
 public:
  Button(int x, int y, int width, int height, SolidRectangleRenderer& renderer);

  ~Button() = default;
  bool isPressed(int x, int y);

 private:
  int mX;
  int mY;
  int mWidth;
  int mHeight;
  SolidRectangle& mRectangle;
};
