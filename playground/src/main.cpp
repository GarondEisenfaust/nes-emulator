#include "RenderContext.h"
#include <array>
#include <iostream>

int main() {
  RenderContext renderContext;

  renderContext.DrawGrid();

  renderContext.Init();
  renderContext.GameLoop([]() {});
  return 0;
}
