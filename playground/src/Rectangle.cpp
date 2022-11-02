#include "Rectangle.h"
#include <cstring>

Rectangle::Rectangle(float x, float y, float width, float height) : mX(x), mY(y), mWidth(width), mHeight(height) {}

Rectangle::TriangleVertices Rectangle::CalculateTriangles() {
  Vertex topLeft{mX, mY, 0};
  Vertex topRight{mX + mWidth, mY, 0};
  Vertex bottomLeft{mX, mY + mHeight, 0};
  Vertex bottomRight{mX + mWidth, mY + mHeight, 0};

  TriangleVertices triangleVertices;
  std::memcpy(&(triangleVertices[0 * NUMBER_OF_COORDINATES]), bottomRight.data(), VERTEX_SIZE);
  std::memcpy(&(triangleVertices[1 * NUMBER_OF_COORDINATES]), bottomLeft.data(), VERTEX_SIZE);
  std::memcpy(&(triangleVertices[2 * NUMBER_OF_COORDINATES]), topLeft.data(), VERTEX_SIZE);

  std::memcpy(&(triangleVertices[3 * NUMBER_OF_COORDINATES]), topLeft.data(), VERTEX_SIZE);
  std::memcpy(&(triangleVertices[4 * NUMBER_OF_COORDINATES]), topRight.data(), VERTEX_SIZE);
  std::memcpy(&(triangleVertices[5 * NUMBER_OF_COORDINATES]), bottomRight.data(), VERTEX_SIZE);

  return triangleVertices;
}
