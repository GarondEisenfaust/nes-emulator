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
}

RenderContext::~RenderContext() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(mWindow);
  glfwTerminate();
}

void RenderContext::GameLoop(std::function<void()> loop) {
  Shader fragmentShader(FragmentShaderCode, GL_FRAGMENT_SHADER);
  Shader vertexShader(VertexShaderCode, GL_VERTEX_SHADER);

  mShaderProgram.AttachShader(fragmentShader);
  mShaderProgram.AttachShader(vertexShader);
  mShaderProgram.Link();
  mShaderProgram.Use();

  float vertices[] = {
      1, -1, 0, 1, 1, 1, 1, 0, 1, 0, -1, 1, 0, 0, 0, -1, -1, 0, 0, 1,
  };

  unsigned int indices[] = {0, 1, 3, 1, 2, 3};

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  while (!glfwWindowShouldClose(mWindow)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    loop();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(mWindow);
  }
}

int RenderContext::GetHeight() { return mHeight; }
int RenderContext::GetWidth() { return mWidth; }

GLFWwindow* RenderContext::GetWindow() { return mWindow; }
