#pragma once
#include "ShaderProgram.h"
#include "TexturedRectangle.h"
#include <memory>
#include <vector>

class TexturedRectangleRenderer {
 public:
  TexturedRectangleRenderer(int screenWidth, int screenHeight);
  TexturedRectangle& CreateRectangle(int x, int y, int width, int height);
  TexturedRectangle& CreateRectangle(int x, int y, int width, int height, const PixelColorF& color);
  void Render();

 private:
  std::vector<std::unique_ptr<TexturedRectangle>> mRectangles;
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
