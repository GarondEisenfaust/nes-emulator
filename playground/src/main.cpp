#include "Definitions.h"
#include "rendering/Grid.h"
#include "rendering/RenderContext.h"
#include <array>
#include <iostream>

int main() {
  RenderContext renderContext;

  Grid grid(WIDTH, HEIGHT);
  grid.Init();

  auto pixels = std::make_shared<Buffer<GLfloat>>(3, 0, GL_STATIC_DRAW);
  auto colors = std::make_shared<Buffer<GLfloat>>(4, 1, GL_DYNAMIC_DRAW);
  auto vertexArray = std::make_shared<VertexArray>();

  auto gridData = grid.MakePixelData();
  pixels->SetData(gridData);

  vertexArray->Attach(colors);
  vertexArray->Attach(pixels);
  vertexArray->SetDrawBuffer(pixels);

  renderContext.AddVertexArray(vertexArray);

  renderContext.GameLoop([&grid, colors]() {
    grid.UpdateColor();
    auto colorData = grid.MakeColorData();
    colors->SetData(colorData);
  });
  return 0;
}
