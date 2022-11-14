#include "RenderContext.h"
#include "Definitions.h"
#include "Rectangle.h"
#include "Shader.h"
#include "Util.h"
#include <boost/dll/runtime_symbol_info.hpp>
#include <cstdlib>
#include <iostream>

std::string GetExecutableDirectory() { return std::string(boost::dll::program_location().parent_path().c_str()); }
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

  auto workDir = GetExecutableDirectory();
  Shader fragmentShader((workDir + "/shaders/FragmentShader.glsl").c_str(), GL_FRAGMENT_SHADER);
  Shader vertexShader((workDir + "/shaders/VertexShader.glsl").c_str(), GL_VERTEX_SHADER);

  mShaderProgram.AttachShader(fragmentShader);
  mShaderProgram.AttachShader(vertexShader);
  mShaderProgram.Link();
}

void RenderContext::Init() {
  glGenVertexArrays(1, &mVAO);
  glGenBuffers(1, &mVBO);
  glGenBuffers(1, &mVBOColors);
}

void RenderContext::UpdateBuffers() {
  mVertices.clear();
  mColors.clear();
  for (auto& shape : mShapes) {
    auto triangles = shape.CalculateTriangles();
    this->AddVertices(triangles);
  };

  glBindVertexArray(mVAO);

  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mVertices.size(), mVertices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ARRAY_BUFFER, mVBOColors);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mColors.size(), mColors.data(), GL_DYNAMIC_DRAW);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

RenderContext::~RenderContext() {
  glDeleteVertexArrays(1, &mVAO);
  glDeleteBuffers(1, &mVBO);
  glDeleteBuffers(1, &mVBOColors);
  glfwTerminate();
}

void RenderContext::AddVertices(Rectangle::TriangleVertices& verticesToAdd) {
  for (auto& vertex : verticesToAdd) {
    mVertices.push_back(vertex.position.x);
    mVertices.push_back(vertex.position.y);
    mVertices.push_back(vertex.position.z);

    mColors.push_back(vertex.color.r);
    mColors.push_back(vertex.color.g);
    mColors.push_back(vertex.color.b);
  }
}

void RenderContext::DrawRectangle(float x, float y, float width, float height, glm::vec3 color) {
  mShapes.emplace_back(x, y, width, height, color, *this);
}

void RenderContext::GameLoop(std::function<void()> loop) {
  while (!glfwWindowShouldClose(mWindow)) {
    UpdateBuffers();

    glfwPollEvents();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mShaderProgram.Use();
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());
    glBindVertexArray(0);
    glfwSwapBuffers(mWindow);

    loop();
  }
}

int RenderContext::GetHeight() { return mHeight; }
int RenderContext::GetWidth() { return mWidth; }

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
}

void RenderContext::DrawGrid() {
  const int gridWidth = 256;
  const int gridHeight = 240;

  const auto rectangleWidth = static_cast<float>(mWidth) / gridWidth;
  const auto rectangleHeight = static_cast<float>(mHeight) / gridHeight;

  for (auto y = 0; y < gridHeight; y++) {
    for (auto x = 0; x < gridWidth; x++) {
      const auto xPos = x * rectangleWidth;
      const auto yPos = y * rectangleHeight;

      auto r = Util::GetRandomFloat();
      auto g = Util::GetRandomFloat();
      auto b = Util::GetRandomFloat();

      DrawRectangle(xPos, yPos, rectangleWidth, rectangleHeight, glm::vec3{r, g, b});
    }
  }
}
