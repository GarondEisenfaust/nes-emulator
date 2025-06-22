#pragma once
#include "ShaderProgram.h"
#include "SolidRectangle.h"
#include <memory>

class SolidRectangleRenderer {
 public:
  SolidRectangleRenderer(int screenWidth, int screenHeight);
  SolidRectangle& CreateRectangle(int x, int y, int width, int height);
  SolidRectangle& CreateRectangle(int x, int y, int width, int height, const PixelColorF& color);
  void Render();
  int GetScreenHeight();
  int GetScreenWidth();

 private:
  std::vector<std::unique_ptr<SolidRectangle>> mRectangles;
  std::unique_ptr<ShaderProgram> mShaderProgram;
  void InitVertexArray();

  int mScreenWidth;
  int mScreenHeight;
  unsigned int mVao;
  unsigned int mVbo;
  float mVertices[20] = {
      -1, -1, 0, 0, 1, 1, -1, 0, 1, 1, 1, 1, 0, 1, 0, -1, 1, 0, 0, 0,
  };
};
