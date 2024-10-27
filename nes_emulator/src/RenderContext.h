#pragma once
#include "IFrameDecoder.h"
#include "IRenderer.h"
#include "PixelColor.h"
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <array>
#include <cstdint>
#include <functional>
#include <jni.h>
#include <memory>

class RenderContext : public IRenderer {
 public:
  ~RenderContext();

  void Init(struct android_app* app);
  void GameLoop(std::function<void()> loop);
  void DrawOneFrame(std::function<void()> loop);
  void SetFrameDecoder(IFrameDecoder* frameDecoder);
  uint8_t* GetTextureDataPointer();

  void SetNesPixel(int x, int y, uint8_t pixel) override;
  void CommitFrame(unsigned int ppuCycle) override;
  void StartNewFrame() override;
  bool FrameComplete() override;
  int GetWidth();
  int GetHeight();

 private:
  int mWidth;
  int mHeight;
  unsigned int mFramePpuCycle;
  static const int mGridWidth = 256;
  static const int mGridHeight = 240;
  std::array<uint16_t, mGridWidth * mGridHeight> mNesFrameData;
  std::array<PixelColorU8, mGridWidth * mGridHeight> mTextureData;
  bool mFrameComplete = false;
  IFrameDecoder* mFrameDecoder;

  EGLDisplay display_ = nullptr;
  EGLSurface surface_ = nullptr;
  EGLContext context_ = nullptr;
};