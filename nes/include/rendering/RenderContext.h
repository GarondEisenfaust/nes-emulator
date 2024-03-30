#pragma once
#include <glad/glad.h>
#include "IFrameDecoder.h"
#include "IRenderer.h"
#include "PixelColor.h"
#include "ShaderProgram.h"
#include <GLFW/glfw3.h>
#include <array>
#include <cstdint>
#include <functional>
#include <memory>

class RenderContext : public IRenderer {
 public:
  RenderContext();
  ~RenderContext();

  void GameLoop(std::function<void()> loop);
  GLFWwindow* GetWindow();
  void SetFrameDecoder(IFrameDecoder* frameDecoder);
  uint8_t* GetTextureDataPointer();

  void SetNesPixel(int x, int y, uint8_t pixel) override;
  void CommitFrame(unsigned int ppuCycle) override;
  void StartNewFrame() override;
  bool FrameComplete() override;

 private:
  int mWidth;
  int mHeight;
  unsigned int mFramePpuCycle;
  static const int mGridWidth = 256;
  static const int mGridHeight = 240;
  std::array<uint16_t, mGridWidth * mGridHeight> mNesFrameData;
  std::array<PixelColor, mGridWidth * mGridHeight> mTextureData;
  bool mFrameComplete = false;
  int mCurrentPixel = 0;
  GLFWwindow* mWindow;
  IFrameDecoder* mFrameDecoder;
  void DisableImguiFiles();
};