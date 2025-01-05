#include "ShapeRendering/SolidRectangleRenderer.h"
#include "../OpenGL.h"
#include "Shader.h"
#include "SolidColorShader.h"
#include "VertexShader.h"

SolidRectangleRenderer::SolidRectangleRenderer(int screenWidth, int screenHeight)
    : mScreenWidth(screenWidth), mScreenHeight(screenHeight) {
  InitVertexArray();
  Shader fragmentShader(SolidColorShader().source, GL_FRAGMENT_SHADER);
  Shader vertexShader(VertexShader().source, GL_VERTEX_SHADER);
  mShaderProgram = std::make_unique<ShaderProgram>();
  mShaderProgram->Use();
  mShaderProgram->AttachShader(fragmentShader);
  mShaderProgram->AttachShader(vertexShader);
  mShaderProgram->Link();
}

SolidRectangle& SolidRectangleRenderer::CreateRectangle(int x, int y, int width, int height) {
  return *mRectangles.emplace_back(std::make_unique<SolidRectangle>(x, y, width, height, mScreenWidth, mScreenHeight));
}

SolidRectangle& SolidRectangleRenderer::CreateRectangle(int x, int y, int width, int height, const PixelColorF& color) {
  return *mRectangles.emplace_back(
      std::make_unique<SolidRectangle>(x, y, width, height, mScreenWidth, mScreenHeight, color));
}

void SolidRectangleRenderer::InitVertexArray() {
  glGenVertexArrays(1, &mVao);
  glBindVertexArray(mVao);

  glGenBuffers(1, &mVbo);
  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(mVertices), mVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);
}

void SolidRectangleRenderer::Render() {
  mShaderProgram->Use();
  glBindVertexArray(mVao);
  for (auto& rectangle : mRectangles) {
    rectangle->UpdateTransformIfNecessary();
  }

  for (const auto& rectangle : mRectangles) {
    mShaderProgram->SetUniform("transform", rectangle->GetTransform());
    mShaderProgram->SetUniform("color", rectangle->GetColor());
    mShaderProgram->SetUniform("highlight", rectangle->GetHighlight());
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  }
  glBindVertexArray(0);
}

int SolidRectangleRenderer::GetScreenHeight() { return mScreenHeight; }

int SolidRectangleRenderer::GetScreenWidth() { return mScreenWidth; }
