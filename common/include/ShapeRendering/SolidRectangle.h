#pragma once
#include "PixelColor.h"
#include <glm/glm.hpp>

class SolidRectangleRenderer;
class SolidRectangle {
 public:
  SolidRectangle(int x, int y, int width, int height, int screenWidth, int screenHeight);
  SolidRectangle(int x, int y, int width, int height, int screenWidth, int screenHeight, const PixelColorF& color);
  ~SolidRectangle();
  void UpdateTransformIfNecessary();
  const glm::mat4& GetTransform() const;
  const PixelColorF& GetColor() const;
  bool GetHighlight() const;
  void Highlight();
  void DontHighlight();

 private:
  int mX;
  int mY;
  int mWidth;
  int mHeight;
  int mScreenWidth;
  int mScreenHeight;
  bool mNeedsUpdate;
  bool mHighlight;
  glm::mat4 mTransform;
  PixelColorF mColor;
};
