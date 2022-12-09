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

  auto ram = std::make_unique<RAM>();
  Bus bus(ram.get());
  Processor6502 cpu;
  PixelProcessingUnit ppu(&grid);

  cpu.ConnectBus(&bus);
  ppu.ConnectBus(&bus);

  auto workDir = Util::GetExecutableDirectory();
  Cartridge cartridge(fmt::format("{}/roms/color_test.nes", workDir));
  bus.InsertCartridge(&cartridge);

  renderContext.GameLoop([&grid, &bus, &ppu, colors]() {
    auto now = std::chrono::system_clock::now();

    RenderCompleteFrame(bus, ppu);
    auto colorData = grid.MakeColorData();
    colors->SetData(colorData);

    {
      using namespace std::chrono_literals;
      auto end = now + (1s / 60);
      std::this_thread::sleep_until(end);
    }
  });
  return 0;
}
