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

Rectangle::Rectangle(float x, float y, float width, float height, glm::vec3 color, RenderContext& renderContext)
    : mX(x), mY(y), mWidth(width), mHeight(height), mRenderContext(renderContext), mColor(color) {}

Rectangle::TriangleVertices Rectangle::CalculateTriangles() {
  auto [x, y, width, height] = CalculateScreenSpace(mX, mY, mWidth, mHeight, mRenderContext);

  Vertex topLeft{{x, y, 0}, mColor};
  Vertex topRight{{x + width, y, 0}, mColor};
  Vertex bottomLeft{{x, y + height, 0}, mColor};
  Vertex bottomRight{{x + width, y + height, 0}, mColor};

  TriangleVertices triangleVertices;
  triangleVertices[0] = bottomRight;
  triangleVertices[1] = bottomLeft;
  triangleVertices[2] = topLeft;
  triangleVertices[3] = topLeft;
  triangleVertices[4] = topRight;
  triangleVertices[5] = bottomRight;

  return triangleVertices;
}
