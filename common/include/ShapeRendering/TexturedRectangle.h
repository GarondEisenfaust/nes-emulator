#pragma once
#include "PixelColor.h"
#include <glm/glm.hpp>

class TexturedRectangleRenderer;
class TexturedRectangle {
 public:
  TexturedRectangle(int x, int y, int width, int height, int screenWidth, int screenHeight);
  TexturedRectangle(int x, int y, int width, int height, int screenWidth, int screenHeight, const PixelColorF& color);
  ~TexturedRectangle();
  void UpdateTransformIfNecessary();
  const glm::mat4& GetTransform() const;
  const PixelColorF& GetColor() const;

 private:
  int mX;
  int mY;
  int mWidth;
  int mHeight;
  int mScreenWidth;
  int mScreenHeight;
  bool mNeedsUpdate;
  glm::mat4 mTransform;
  PixelColorF mColor;
};
