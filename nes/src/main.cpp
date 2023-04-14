#include "Bus.h"
#include "Cartridge.h"
#include "Controller.h"
#include "Cpu.h"
#include "Definitions.h"
#include "Ppu.h"
#include "Ram.h"
#include "fmt/format.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "rendering/Grid.h"
#include "rendering/RenderContext.h"
#include <array>
#include <chrono>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>

void RenderCompleteFrame(Bus& bus, Grid& grid) {
  while (!grid.FrameComplete()) {
    bus.Clock();
  }
  grid.StartNewFrame();
}

void MakeOneStep(Bus& bus) {
  auto finished = false;

  do {
    bus.Clock();
  } while (bus.mCpu->cycles > 0);

  do {
    bus.Clock();
  } while (bus.mCpu->cycles <= 0);
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cout << "You have to specify a ROM to load!" << std::endl;
    return 1;
  }

  RenderContext renderContext;
  Grid grid(WIDTH, HEIGHT, 256, 240);
  grid.Init();

  auto pixels = std::make_shared<Buffer<GLfloat>>(0, 3, GL_STATIC_DRAW);
  auto colors = std::make_shared<Buffer<GLfloat>>(1, 4, GL_DYNAMIC_DRAW);
  auto vertexArray = std::make_shared<VertexArray>();

  auto gridData = grid.MakePixelData();
  pixels->SetData(gridData);

  vertexArray->Attach(colors);
  vertexArray->Attach(pixels);
  vertexArray->SetDrawBuffer(pixels);

  renderContext.AddVertexArray(vertexArray);

  auto ram = std::make_unique<Ram>();
  Bus bus(*ram);
  Cpu cpu;
  Ppu ppu(grid);
  Controller controller(renderContext.GetWindow());

  bus.ConnectController(&controller);
  cpu.ConnectBus(&bus);
  ppu.ConnectBus(&bus);

  std::string romPath = argv[1];
  Cartridge cartridge(romPath);
  bus.InsertCartridge(&cartridge);
  bus.Reset();

  using namespace std::chrono_literals;
  auto diff = (1000ms / 60);
  renderContext.GameLoop([&]() {
    static auto next = std::chrono::system_clock::now();
    controller.SetControllerBitBasedOnInput(GLFW_JOYSTICK_1);
    controller.SetControllerBitBasedOnInput(GLFW_JOYSTICK_2);

    auto colorData = grid.MakeColorData();
    colors->SetData(colorData);

    RenderCompleteFrame(bus, grid);
    std::this_thread::sleep_until(next);
    next += diff;
  });
  return 0;
}
