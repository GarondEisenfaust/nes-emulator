#include <GL/glew.h>
#include <array>
#include <memory>

#define NUMBER_OF_COORDINATES 3
#define NUMBER_OF_VERTICES 6
#define VERTEX_SIZE sizeof(float) * NUMBER_OF_COORDINATES

class Rectangle {
 public:
  using TriangleVertices = std::array<float, NUMBER_OF_VERTICES * NUMBER_OF_COORDINATES>;
  using Vertex = std::array<float, NUMBER_OF_COORDINATES>;

  Rectangle(float x, float y, float width, float height);

  TriangleVertices CalculateTriangles();

 private:
  float mX;
  float mY;
  float mWidth;
  float mHeight;
};