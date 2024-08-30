#include "RenderContext.h"
#include "../AndroidOut.h"
#include "Definitions.h"
#include "Shader.h"
#include "Surface.h"
#include "Texture.h"
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <game-activity/GameActivity.cpp>
#include <game-activity/native_app_glue/android_native_app_glue.c>
#include <game-text-input/gametextinput.cpp>
#include <cassert>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <thread>

#define PRINT_GL_STRING(s) \
  { aout << #s ": " << glGetString(s) << std::endl; }
#define PRINT_GL_STRING_AS_LIST(s)                                                              \
  {                                                                                             \
    std::istringstream extensionStream((const char*)glGetString(s));                            \
    std::vector<std::string> extensionList(std::istream_iterator<std::string>{extensionStream}, \
                                           std::istream_iterator<std::string>());               \
    aout << #s ":\n";                                                                           \
    for (auto& extension : extensionList) {                                                     \
      aout << extension << "\n";                                                                \
    }                                                                                           \
    aout << std::endl;                                                                          \
  }
#define CORNFLOWER_BLUE 100 / 255.f, 149 / 255.f, 237 / 255.f, 1

void RenderContext::Init(struct android_app* app) {
  constexpr EGLint attribs[] = {EGL_RENDERABLE_TYPE,
                                EGL_OPENGL_ES3_BIT,
                                EGL_SURFACE_TYPE,
                                EGL_WINDOW_BIT,
                                EGL_BLUE_SIZE,
                                8,
                                EGL_GREEN_SIZE,
                                8,
                                EGL_RED_SIZE,
                                8,
                                EGL_DEPTH_SIZE,
                                24,
                                EGL_NONE};

  // The default display is probably what you want on Android
  auto display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  eglInitialize(display, nullptr, nullptr);

  // figure out how many configs there are
  EGLint numConfigs;
  eglChooseConfig(display, attribs, nullptr, 0, &numConfigs);

  // get the list of configurations
  std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);
  eglChooseConfig(display, attribs, supportedConfigs.get(), numConfigs, &numConfigs);

  // Find a config we like.
  // Could likely just grab the first if we don't care about anything else in the config.
  // Otherwise hook in your own heuristic
  auto config =
      *std::find_if(supportedConfigs.get(), supportedConfigs.get() + numConfigs, [&display](const EGLConfig& config) {
        EGLint red, green, blue, depth;
        if (eglGetConfigAttrib(display, config, EGL_RED_SIZE, &red) &&
            eglGetConfigAttrib(display, config, EGL_GREEN_SIZE, &green) &&
            eglGetConfigAttrib(display, config, EGL_BLUE_SIZE, &blue) &&
            eglGetConfigAttrib(display, config, EGL_DEPTH_SIZE, &depth)) {
          aout << "Found config with " << red << ", " << green << ", " << blue << ", " << depth << std::endl;
          return red == 8 && green == 8 && blue == 8 && depth == 24;
        }
        return false;
      });

  aout << "Found " << numConfigs << " configs" << std::endl;
  aout << "Chose " << config << std::endl;

  // create the proper window surface
  EGLint format;
  eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

  EGLSurface surface = eglCreateWindowSurface(display, config, app->window, nullptr);

  // Create a GLES 3 context
  EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
  EGLContext context = eglCreateContext(display, config, nullptr, contextAttribs);

  // get some window metrics
  auto madeCurrent = eglMakeCurrent(display, surface, surface, context);
  assert(madeCurrent);

  display_ = display;
  surface_ = surface;
  context_ = context;

  PRINT_GL_STRING(GL_VENDOR);
  PRINT_GL_STRING(GL_RENDERER);
  PRINT_GL_STRING(GL_VERSION);
  PRINT_GL_STRING_AS_LIST(GL_EXTENSIONS);

  // setup any other gl related global states
  glClearColor(CORNFLOWER_BLUE);

  // enable alpha globally for now, you probably don't want to do this in a game
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  EGLint width;
  eglQuerySurface(display_, surface_, EGL_WIDTH, &mWidth);

  EGLint height;
  eglQuerySurface(display_, surface_, EGL_HEIGHT, &mHeight);
  glViewport(0, 0, mWidth, mHeight);

  mInitialized = true;
}

RenderContext::~RenderContext() {
  if (display_ != EGL_NO_DISPLAY) {
    eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (context_ != EGL_NO_CONTEXT) {
      eglDestroyContext(display_, context_);
      context_ = EGL_NO_CONTEXT;
    }
    if (surface_ != EGL_NO_SURFACE) {
      eglDestroySurface(display_, surface_);
      surface_ = EGL_NO_SURFACE;
    }
    eglTerminate(display_);
    display_ = EGL_NO_DISPLAY;
  }
}

void RenderContext::GameLoop(std::function<void()> loop) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  while (true) {
    loop();

    glClear(GL_COLOR_BUFFER_BIT);
    mFrameDecoder->DecodeAndDraw(mNesFrameData.data(), mFramePpuCycle);

    eglSwapBuffers(display_, surface_);
  }
}

void RenderContext::SetNesPixel(int x, int y, uint8_t pixel) {
  if ((0 <= x && x < mGridWidth) && (0 <= y && y < mGridHeight)) {
    const size_t index = y * mGridWidth + x;
    mNesFrameData[index] = pixel;
  }
}

void RenderContext::SetFrameDecoder(IFrameDecoder* frameDecoder) { mFrameDecoder = frameDecoder; }

void RenderContext::CommitFrame(unsigned int ppuCycle) {
  mFramePpuCycle = ppuCycle;
  mFrameComplete = true;
}

void RenderContext::StartNewFrame() { mFrameComplete = false; }

bool RenderContext::FrameComplete() { return mFrameComplete; }

uint8_t* RenderContext::GetTextureDataPointer() { return reinterpret_cast<uint8_t*>(mTextureData.data()); }
