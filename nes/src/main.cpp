#include "Bus.h"
#include "Cartridge.h"
#include "Controller.h"
#include "Cpu.h"
#include "Definitions.h"
#include "Ppu.h"
#include "fmt/format.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "rendering/Grid.h"
#include "rendering/RenderContext.h"
#include "windows/DisassamblerWindow.h"
#include "windows/RegisterWindow.h"
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

int main() {
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

  auto ram = std::make_unique<RAM>();
  Bus bus(*ram);
  Cpu cpu;
  Ppu ppu(grid);
  Controller controller(renderContext.GetWindow());

  bus.ConnectController(&controller);
  cpu.ConnectBus(&bus);
  ppu.ConnectBus(&bus);

  auto workDir = std::filesystem::current_path().string();
  Cartridge cartridge(fmt::format("{}/roms/smb.nes", workDir));
  bus.InsertCartridge(&cartridge);
  bus.Reset();

  auto stepMode = false;
  auto shouldStep = false;
  RenderContext::KeyCallback keyCallback = [&](GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
      stepMode = !stepMode;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && stepMode) {
      shouldStep = true;
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
      const auto palette = 1;
      ppu.WritePatternTableToImage("table-1.png", 1, palette);
      ppu.WritePatternTableToImage("table-2.png", 2, palette);
      ppu.WriteColorPaletteToImage("color-palette.png");
    }
  };

  renderContext.SetKeyCallback(&keyCallback);
  DisassamblerWindow disassamblerWindow(cpu);
  RegisterWindow registerWindow(cpu);
  {
    using namespace std::chrono_literals;
    auto diff = (1000ms / 60);
    renderContext.GameLoop([&]() {
      static auto next = std::chrono::system_clock::now();
      controller.SetControllerBitBasedOnInput();
      auto colorData = grid.MakeColorData();
      colors->SetData(colorData);
      registerWindow.Render();
      disassamblerWindow.Render();

      if (stepMode) {
        if (shouldStep) {
          MakeOneStep(bus);
          shouldStep = false;
        }
      } else {
        RenderCompleteFrame(bus, grid);
        std::this_thread::sleep_until(next);
        next += diff;
      }
    });
  }
  return 0;
}
