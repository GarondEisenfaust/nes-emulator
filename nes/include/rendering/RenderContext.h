#pragma once
#include "ShaderProgram.h"
#include "glm/vec3.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <functional>
#include <memory>
#include <vector>

class RenderContext {
 public:
  using KeyCallback = std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)>;
  RenderContext();
  ~RenderContext();

  void GameLoop(std::function<void()> loop);
  int GetHeight();
  int GetWidth();
  GLFWwindow* GetWindow();

 private:
  int mWidth;
  int mHeight;
  GLFWwindow* mWindow;

  ShaderProgram mShaderProgram;
};