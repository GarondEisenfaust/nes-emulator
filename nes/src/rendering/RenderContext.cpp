#include "rendering/RenderContext.h"
#include "Definitions.h"
#include "FragmentShader.h"
#include "VertexShader.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "rendering/Shader.h"
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <thread>

RenderContext::RenderContext() : mShaderProgram() {
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

  glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* callback = (KeyCallback*)glfwGetWindowUserPointer(window);
    if (callback) {
      (*callback)(window, key, scancode, action, mods);
    }
  });
  glewInit();

  glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
  glViewport(0, 0, mWidth, mHeight);

  auto workDir = std::filesystem::current_path().string();
  Shader fragmentShader(FragmentShaderCode, GL_FRAGMENT_SHADER);
  Shader vertexShader(VertexShaderCode, GL_VERTEX_SHADER);

  mShaderProgram.AttachShader(fragmentShader);
  mShaderProgram.AttachShader(vertexShader);
  mShaderProgram.Link();
}

void RenderContext::UpdateBuffers() {
  for (auto vertexArray : mVertexArrays) {
    vertexArray->Upload();
  }
}

RenderContext::~RenderContext() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(mWindow);
  glfwTerminate();
}

void RenderContext::GameLoop(std::function<void()> loop) {
  while (!glfwWindowShouldClose(mWindow)) {
    UpdateBuffers();

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mShaderProgram.Use();
    mShaderProgram.SetUniform("height", HEIGHT);
    mShaderProgram.SetUniform("width", WIDTH);

    for (auto vertexArray : mVertexArrays) {
      vertexArray->Draw();
    }

    loop();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(mWindow);
  }
}

int RenderContext::GetHeight() { return mHeight; }
int RenderContext::GetWidth() { return mWidth; }

void RenderContext::SetKeyCallback(KeyCallback* keyCallback) { glfwSetWindowUserPointer(mWindow, keyCallback); }

void RenderContext::AddVertexArray(std::shared_ptr<VertexArray> vertexArray) { mVertexArrays.push_back(vertexArray); }

GLFWwindow* RenderContext::GetWindow() { return mWindow; }
