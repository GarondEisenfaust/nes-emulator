#include "Rectangle.h"
#include "Definitions.h"
#include "RenderContext.h"
#include <cstring>

std::tuple<float, float, float, float> CalculateScreenSpace(float x, float y, float width, float height,
                                                            RenderContext& renderContext) {
  float screenX = ((2 * x / renderContext.GetWidth()) - 1);
  float screenY = ((2 * y / renderContext.GetHeight()) - 1);
  float screenWidth = (2 * width / renderContext.GetWidth());
  float screenHeight = (2 * height / renderContext.GetHeight());
  return {screenX, screenY, screenWidth, screenHeight};
}

Rectangle::Rectangle(float x, float y, float width, float height, RenderContext& renderContext)
    : mX(x), mY(y), mWidth(width), mHeight(height), mRenderContext(renderContext) {}

Rectangle::TriangleVertices Rectangle::CalculateTriangles() {
  auto [x, y, width, height] = CalculateScreenSpace(mX, mY, mWidth, mHeight, mRenderContext);

  Vertex topLeft{x, y, 0};
  Vertex topRight{x + width, y, 0};
  Vertex bottomLeft{x, y + height, 0};
  Vertex bottomRight{x + width, y + height, 0};

  TriangleVertices triangleVertices;
  std::memcpy(&(triangleVertices[0 * NUMBER_OF_COORDINATES]), bottomRight.data(), VERTEX_SIZE);
  std::memcpy(&(triangleVertices[1 * NUMBER_OF_COORDINATES]), bottomLeft.data(), VERTEX_SIZE);
  std::memcpy(&(triangleVertices[2 * NUMBER_OF_COORDINATES]), topLeft.data(), VERTEX_SIZE);

  std::memcpy(&(triangleVertices[3 * NUMBER_OF_COORDINATES]), topLeft.data(), VERTEX_SIZE);
  std::memcpy(&(triangleVertices[4 * NUMBER_OF_COORDINATES]), topRight.data(), VERTEX_SIZE);
  std::memcpy(&(triangleVertices[5 * NUMBER_OF_COORDINATES]), bottomRight.data(), VERTEX_SIZE);

  return triangleVertices;
}
