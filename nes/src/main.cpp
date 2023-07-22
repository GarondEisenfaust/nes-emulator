#include "Apu.h"
#include "Bus.h"
#include "Cartridge.h"
#include "Controller.h"
#include "Cpu.h"
#include "Definitions.h"
#include "Miniaudio.h"
#include "Ppu.h"
#include "Ram.h"
#include "RingBuffer.h"
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

Grid grid(WIDTH, HEIGHT, 256, 240);

float Normalize(float value, float min, float max, float minDesired = -1, float maxDesired = 1) {
  auto firstPart = (value - min) / (max - min);
  auto range = maxDesired - minDesired;
  return firstPart * range + minDesired;
}

float minReceivedSample = 0;
float maxReceivedSample = 0;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
  auto* asFloatPointer = reinterpret_cast<float*>(pOutput);
  auto* ringBuffer = reinterpret_cast<RingBuffer*>(pDevice->pUserData);
  for (int i = 0; i < frameCount; i++) {
    auto value = ringBuffer->Read();
    minReceivedSample = std::min(value, minReceivedSample);
    maxReceivedSample = std::max(value, maxReceivedSample);
    asFloatPointer[i] = Normalize(value, minReceivedSample, maxReceivedSample);
  }
};

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
  RingBuffer ringBuffer(1470);
  Apu apu(ringBuffer);

  Controller controller(renderContext.GetWindow());

  bus.ConnectController(&controller);
  cpu.ConnectBus(&bus);
  ppu.ConnectBus(&bus);
  apu.ConnectBus(&bus);

  std::string romPath = argv[1];
  Cartridge cartridge(romPath);
  bus.InsertCartridge(&cartridge);
  bus.Reset();

  ma_device_config deviceConfig;
  ma_device device;

  deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = DEVICE_FORMAT;
  deviceConfig.playback.channels = DEVICE_CHANNELS;
  deviceConfig.sampleRate = DEVICE_SAMPLE_RATE;
  deviceConfig.pUserData = reinterpret_cast<void*>(&ringBuffer);
  deviceConfig.dataCallback = data_callback;

  if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
    printf("Failed to open playback device.\n");
    return -4;
  }

  printf("Device Name: %s\n", device.playback.name);

  if (ma_device_start(&device) != MA_SUCCESS) {
    printf("Failed to start playback device.\n");
    ma_device_uninit(&device);
    return -5;
  }

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
