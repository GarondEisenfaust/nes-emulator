#include "RenderContext.h"
#include <array>
#include <iostream>

int main() {
  RenderContext renderContext;

  std::array<Rectangle, 8> triangles = {
      Rectangle(0, 0, 50, 50, renderContext),     Rectangle(50, 50, 50, 50, renderContext),
      Rectangle(100, 100, 50, 50, renderContext), Rectangle(100, 0, 50, 50, renderContext),
      Rectangle(0, 100, 50, 50, renderContext),   Rectangle(0, 200, 50, 50, renderContext),
      Rectangle(100, 200, 50, 50, renderContext), Rectangle(50, 150, 50, 50, renderContext)};

  renderContext.Init();
  renderContext.GameLoop([]() {});
  return 0;
}
