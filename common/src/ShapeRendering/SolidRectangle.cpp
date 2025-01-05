#include "ShapeRendering/SolidRectangle.h"
#include <glm/gtc/matrix_transform.hpp>

SolidRectangle::SolidRectangle(int x, int y, int width, int height, int screenWidth, int screenHeight)
    : SolidRectangle(x, y, width, height, screenWidth, screenHeight, {0, 0, 0, 1}) {
  UpdateTransformIfNecessary();
}

SolidRectangle::SolidRectangle(int x, int y, int width, int height, int screenWidth, int screenHeight,
                               const PixelColorF& color)
    : mX(x),
      mY(y),
      mWidth(width),
      mHeight(height),
      mScreenWidth(screenWidth),
      mScreenHeight(screenHeight),
      mColor(color),
      mNeedsUpdate(true),
      mHighlight(false) {}

SolidRectangle::~SolidRectangle() {}

inline float Normalize(float value, float min, float max, float minDesired = -1.0, float maxDesired = 1.0) {
  const auto firstPart = (value - min) / (max - min);
  const auto range = maxDesired - minDesired;
  return firstPart * range + minDesired;
}

void SolidRectangle::UpdateTransformIfNecessary() {
  if (!mNeedsUpdate) {
    return;
  }

  const float topRightX = mX + (mWidth / 2);
  const float topRightY = mY + (mHeight / 2);

  const float normalizedX = Normalize(topRightX, 0, mScreenWidth, -1, 1);
  const float normalizedY = Normalize(topRightY, 0, mScreenHeight, -1, 1);
  const float normalizedWidth = Normalize(mWidth, 0, mScreenWidth, 0, 1);
  const float normalizedHeight = Normalize(mHeight, 0, mScreenHeight, 0, 1);

  const auto translate = glm::translate(glm::identity<glm::mat4>(), glm::vec3(normalizedX, normalizedY, 1));
  const auto scale = glm::scale(glm::identity<glm::mat4>(), glm::vec3(normalizedWidth, normalizedHeight, 1));
  mTransform = translate * scale;
  mNeedsUpdate = false;
}

const glm::mat4& SolidRectangle::GetTransform() const { return mTransform; }

const PixelColorF& SolidRectangle::GetColor() const { return mColor; }

bool SolidRectangle::GetHighlight() const { return mHighlight; }

void SolidRectangle::Highlight() { mHighlight = true; }

void SolidRectangle::DontHighlight() { mHighlight = false; }
