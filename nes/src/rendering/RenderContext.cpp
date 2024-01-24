#include "rendering/RenderContext.h"
#include "Definitions.h"
#include "Surface.h"
#include "Texture.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "rendering/RenderContext.h"
#include "rendering/Shader.h"
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <thread>

RenderContext::RenderContext() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  mWindow = glfwCreateWindow(WIDTH, HEIGHT, APPLICATION_NAME, nullptr, nullptr);
  glfwMakeContextCurrent(mWindow);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
  ImGui_ImplOpenGL3_Init();

  glewInit();

  glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
  glViewport(0, 0, mWidth, mHeight);
}

RenderContext::~RenderContext() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(mWindow);
  glfwTerminate();
}

void RenderContext::GameLoop(std::function<void()> loop) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  while (!glfwWindowShouldClose(mWindow)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    loop();

    glClear(GL_COLOR_BUFFER_BIT);
    mFrameDecoder->DecodeAndDraw(mNesFrameData.data(), mFramePpuCycle);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(mWindow);
  }
}

GLFWwindow* RenderContext::GetWindow() { return mWindow; }

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
