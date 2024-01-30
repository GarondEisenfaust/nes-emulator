#include "Apu.h"
#include "ArgumentParser.h"
#include "Bus.h"
#include "Cartridge.h"
#include "Controller.h"
#include "Cpu.h"
#include "Definitions.h"
#include "ForegroundRenderer.h"
#include "Miniaudio.h"
#include "Ppu.h"
#include "Ram.h"
#include "RingBuffer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "rendering/LookupTableFrameDecoder.h"
#include "rendering/NtscSignalFrameDecoder.h"
#include "rendering/NtscSignalFrameDecoderGpu.h"
#include "rendering/RenderContext.h"
#include <array>
#include <chrono>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>
#include <thread>

float Normalize(float value, float min, float max, float minDesired = -1, float maxDesired = 1) {
  auto firstPart = (value - min) / (max - min);
  auto range = maxDesired - minDesired;
  return firstPart * range + minDesired;
}

float minReceivedSample = 0;
float maxReceivedSample = 0.001;

void AudioCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
  auto* asFloatPointer = reinterpret_cast<float*>(pOutput);
  auto* ringBuffer = reinterpret_cast<RingBuffer*>(pDevice->pUserData);
  for (int i = 0; i < frameCount; i++) {
    auto value = ringBuffer->Read();
    minReceivedSample = std::min(value, minReceivedSample);
    maxReceivedSample = std::max(value, maxReceivedSample);
    asFloatPointer[i] = Normalize(value, minReceivedSample, maxReceivedSample);
  }
};

void RenderCompleteFrame(Bus& bus, IRenderer& renderer) {
  while (!renderer.FrameComplete()) {
    bus.Clock();
  }
  renderer.StartNewFrame();
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

std::unique_ptr<IFrameDecoder> CreateDecoder(NesConfig::SignalDecoder decoder) {
  switch (decoder) {
    case NesConfig::SignalDecoder::Ntsc: {
      return std::make_unique<NtscSignalFrameDecoder>();
    }
    case NesConfig::SignalDecoder::NtscGpu: {
      return std::make_unique<NtscSignalFrameDecoderGpu>();
    }
    case NesConfig::SignalDecoder::LookupTable:
    default: {
      return std::make_unique<LookupTableFrameDecoder>();
    }
  }
}

int main(int argc, char* argv[]) {
  ArgumentParser argumentParser;
  NesConfig config = argumentParser.Parse(argc, argv);
  if (!config.Validate()) {
    std::terminate();
  }

  RenderContext renderContext;
  std::unique_ptr<IFrameDecoder> decoder = CreateDecoder(config.mDecoder);
  renderContext.SetFrameDecoder(decoder.get());

  auto ram = std::make_unique<Ram>();
  Bus bus(*ram);
  Cpu cpu;
  Ppu ppu(renderContext);
  RingBuffer ringBuffer(2000);
  Apu apu(ringBuffer);

  ForegroundRenderer foregroundRenderer;
  foregroundRenderer.SetPpu(&ppu);
  ppu.SetForegroundRenderer(&foregroundRenderer);

  BackgroundRenderer backgroundRenderer;
  backgroundRenderer.SetPpu(&ppu);
  ppu.SetBackgroundRenderer(&backgroundRenderer);

  Controller controller(renderContext.GetWindow());

  bus.ConnectController(&controller);
  cpu.ConnectBus(&bus);
  ppu.ConnectBus(&bus);
  apu.ConnectBus(&bus);

  Cartridge cartridge(config.mRomPath);
  bus.InsertCartridge(&cartridge);
  bus.Reset();

  ma_device_config deviceConfig;
  ma_device device;

  deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = DEVICE_FORMAT;
  deviceConfig.playback.channels = DEVICE_CHANNELS;
  deviceConfig.sampleRate = DEVICE_SAMPLE_RATE;
  deviceConfig.pUserData = reinterpret_cast<void*>(&ringBuffer);
  deviceConfig.dataCallback = AudioCallback;

  if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
    std::cout << "Failed to open playback device.\n";
    return -4;
  }

  std::cout << "Device Name: " << device.playback.name << "\n";

  if (ma_device_start(&device) != MA_SUCCESS) {
    std::cout << "Failed to start playback device.\n";
    ma_device_uninit(&device);
    return -5;
  }

  using namespace std::chrono_literals;
  const auto diff = (1000ms / 60);
  auto next = std::chrono::system_clock::now();
  renderContext.GameLoop([&]() {
    controller.SetControllerBitBasedOnInput(GLFW_JOYSTICK_1);
    controller.SetControllerBitBasedOnInput(GLFW_JOYSTICK_2);
    RenderCompleteFrame(bus, renderContext);
    std::this_thread::sleep_until(next);
    next += diff;
  });
  return 0;
}
