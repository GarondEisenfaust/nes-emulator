#include "Rectangle.h"
#include <cstring>

Rectangle::Rectangle(float x, float y, float width, float height) : mX(x), mY(y), mWidth(width), mHeight(height) {}

Rectangle::TriangleVertices Rectangle::CalculateTriangles() {
  std::array<float, 3> topLeft{mX, mY, 0};
  std::array<float, 3> topRight{mX + mWidth, mY, 0};
  std::array<float, 3> bottomLeft{mX, mY + mHeight, 0};
  std::array<float, 3> bottomRight{mX + mWidth, mY + mHeight, 0};

  TriangleVertices triangleVertices;
  std::memcpy(&(triangleVertices[0 * 3]), topLeft.data(), sizeof(float) * 3);
  std::memcpy(&(triangleVertices[1 * 3]), topRight.data(), sizeof(float) * 3);
  std::memcpy(&(triangleVertices[2 * 3]), bottomLeft.data(), sizeof(float) * 3);

  std::memcpy(&(triangleVertices[3 * 3]), bottomRight.data(), sizeof(float) * 3);
  std::memcpy(&(triangleVertices[4 * 3]), topRight.data(), sizeof(float) * 3);
  std::memcpy(&(triangleVertices[5 * 3]), topLeft.data(), sizeof(float) * 3);

  return triangleVertices;
}
