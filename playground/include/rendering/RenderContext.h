#pragma once
#include "ShaderProgram.h"
#include "glm/vec3.hpp"
#include "rendering/Buffer.h"
#include "rendering/IBuffer.h"
#include "rendering/VertexArray.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <functional>
#include <memory>
#include <vector>

class RenderContext {
 public:
  RenderContext();
  ~RenderContext();

  void GameLoop(std::function<void()> loop);
  int GetHeight();
  int GetWidth();
  void AddVertexArray(std::shared_ptr<VertexArray> vertexArray);

 private:
  int mWidth;
  int mHeight;
  GLFWwindow* mWindow;

  ShaderProgram mShaderProgram;
  std::vector<std::shared_ptr<VertexArray>> mVertexArrays;

  void UpdateBuffers();
};