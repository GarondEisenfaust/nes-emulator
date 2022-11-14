#pragma once

#include "Vertex.h"
#include "glm/vec3.hpp"
#include <GL/glew.h>
#include <array>
#include <memory>

#define NUMBER_OF_VERTICES 6

class RenderContext;

class Rectangle {
 public:
  using TriangleVertices = std::array<Vertex, NUMBER_OF_VERTICES>;

  Rectangle(float x, float y, float width, float height, glm::vec3 color, RenderContext& renderContext);
  ~Rectangle() = default;

  TriangleVertices CalculateTriangles();

 private:
  float mX;
  float mY;
  float mWidth;
  float mHeight;
  glm::vec3 mColor;
  RenderContext& mRenderContext;
};