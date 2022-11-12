#pragma once
#include "Rectangle.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <functional>
#include <vector>

class RenderContext {
 public:
  RenderContext();
  ~RenderContext();
  void Init();
  void AddVertices(Rectangle::TriangleVertices& verticesToAdd);
  void DrawRectangle(float x, float y, float width, float height);

  void DrawGrid();
  void GameLoop(std::function<void()> loop);
  int GetHeight();
  int GetWidth();

 private:
  int mWidth;
  int mHeight;
  GLFWwindow* mWindow;
  GLuint mShaderProgram;
  std::unique_ptr<std::vector<float>> mVertices;
  std::unique_ptr<std::vector<Rectangle>> mShapes;
  GLuint mVBO;
  GLuint mVAO;

  void UpdateBuffers();
};