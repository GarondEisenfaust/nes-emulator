#include <GL/glew.h>
#include <array>

class Rectangle {
  using TriangleVertices = std::array<float, 6 * 3>;

 public:
  Rectangle(float x, float y, float width, float height);

  TriangleVertices CalculateTriangles();

 private:
  float mX;
  float mY;
  float mWidth;
  float mHeight;
};