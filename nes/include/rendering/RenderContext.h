#pragma once
#include "IRenderer.h"
#include "PixelColor.h"
#include "ShaderProgram.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <functional>
#include <memory>

class RenderContext : public IRenderer {
 public:
  using KeyCallback = std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)>;
  RenderContext();
  ~RenderContext();

  void GameLoop(std::function<void()> loop);
  GLFWwindow* GetWindow();
  void SetPixelColor(int x, int y, PixelColor& color);
  void CommitFrame();
  void StartNewFrame();
  bool FrameComplete();
  void SetData(int index, PixelColor color);
  uint8_t* GetTextureDataPointer();

 private:
  int mWidth;
  int mHeight;
  static const int mGridWidth = 256;
  static const int mGridHeight = 240;
  std::array<PixelColor, mGridWidth * mGridHeight> mTextureData;
  bool mFrameComplete = false;
  int mCurrentPixel = 0;
  GLFWwindow* mWindow;
  ShaderProgram mShaderProgram;
};