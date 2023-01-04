#include "Bus.h"
#include "Cartridge.h"
#include "Definitions.h"
#include "Grid.h"
#include "PixelProcessingUnit.h"
#include "Processor6502.h"
#include "Util.h"
#include "fmt/format.h"
#include "rendering/RenderContext.h"
#include <array>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

void RenderCompleteFrame(Bus& bus, PixelProcessingUnit& ppu) {
  while (!ppu.mFrameComplete) {
    bus.Clock();
  }
  ppu.mFrameComplete = false;
}

int main() {
  RenderContext renderContext;

  Grid grid(WIDTH, HEIGHT, 256, 240);
  grid.Init();

  auto pixels = std::make_shared<Buffer<GLfloat>>(0, 3, GL_STATIC_DRAW);
  auto colors = std::make_shared<Buffer<GLfloat>>(1, 3, GL_DYNAMIC_DRAW);
  auto vertexArray = std::make_shared<VertexArray>();

  auto gridData = grid.MakePixelData();
  pixels->SetData(gridData);

  vertexArray->Attach(colors);
  vertexArray->Attach(pixels);
  vertexArray->SetDrawBuffer(pixels);

  renderContext.AddVertexArray(vertexArray);

  auto ram = std::make_unique<RAM>();
  Bus bus(ram.get());
  Processor6502 cpu;
  PixelProcessingUnit ppu(&grid);

  cpu.ConnectBus(&bus);
  ppu.ConnectBus(&bus);

  auto workDir = Util::GetExecutableDirectory();
  Cartridge cartridge(fmt::format("{}/roms/color_test.nes", workDir));
  bus.InsertCartridge(&cartridge);
  bus.Reset();

  renderContext.GameLoop([&grid, &bus, &ppu, colors]() {
    using namespace std::chrono_literals;
    auto diff = (1000ms / 60);
    auto now = std::chrono::system_clock::now();
    auto next = now + diff;

    RenderCompleteFrame(bus, ppu);
    auto colorData = grid.MakeColorData();
    colors->SetData(colorData);

    std::this_thread::sleep_until(next);
  });
  return 0;
}
