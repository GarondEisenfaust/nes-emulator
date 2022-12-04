#include "rendering/RenderContext.h"
#include "Definitions.h"
#include "Util.h"
#include "rendering/Shader.h"
#include <cstdlib>
#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

RenderContext::RenderContext() : mShaderProgram() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  mWindow = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
  glfwMakeContextCurrent(mWindow);

  glfwSetKeyCallback(mWindow, key_callback);

  glewExperimental = GL_TRUE;
  glewInit();

  glfwGetFramebufferSize(mWindow, &mWidth, &mHeight);
  glViewport(0, 0, mWidth, mHeight);

  auto workDir = Util::GetExecutableDirectory();
  Shader fragmentShader((workDir + "/shaders/FragmentShader.glsl").c_str(), GL_FRAGMENT_SHADER);
  Shader vertexShader((workDir + "/shaders/VertexShader.glsl").c_str(), GL_VERTEX_SHADER);

  mShaderProgram.AttachShader(fragmentShader);
  mShaderProgram.AttachShader(vertexShader);
  mShaderProgram.Link();
}

void RenderContext::UpdateBuffers() {
  for (auto vertexArray : mVertexArrays) {
    vertexArray->Upload();
  }
}

RenderContext::~RenderContext() { glfwTerminate(); }

void RenderContext::GameLoop(std::function<void()> loop) {
  while (!glfwWindowShouldClose(mWindow)) {
    UpdateBuffers();

    glfwPollEvents();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mShaderProgram.Use();

    for (auto vertexArray : mVertexArrays) {
      vertexArray->Draw();
    }

    glfwSwapBuffers(mWindow);

    loop();
  }
}

int RenderContext::GetHeight() { return mHeight; }
int RenderContext::GetWidth() { return mWidth; }

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
}

void RenderContext::AddVertexArray(std::shared_ptr<VertexArray> vertexArray) { mVertexArrays.push_back(vertexArray); }
