#include "Apu.h"
#include "ArgumentParser.h"
#include "AudioDevice.h"
#include "BackgroundRenderer.h"
#include "Bus.h"
#include "Controller.h"
#include "Cpu.h"
#include "ForegroundRenderer.h"
#include "IRenderer.h"
#include "LoadRomWindow.h"
#include "NesConfig.h"
#include "Ppu.h"
#include "rendering/IFrameDecoder.h"
#include "rendering/LookupTableFrameDecoder.h"
#include "rendering/NtscSignalFrameDecoder.h"
#include "rendering/NtscSignalFrameDecoderGpu.h"
#include "rendering/RenderContext.h"
#include <chrono>
#include <thread>

void RenderCompleteFrame(Bus& bus, IRenderer& renderer) {
  if (!bus.CartridgeInserted()) {
    return;
  }
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
  AudioDevice audioDevice;
  Apu apu(audioDevice);

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

  if (!config.mRomPath.empty()) {
    bus.InsertCartridge(std::make_shared<Cartridge>(config.mRomPath));
    bus.Reset();
  }
  LoadRomWindow romWindow("roms", config.mRomDirPath.c_str(), &bus);

  using namespace std::chrono_literals;
  const auto diff = (1000ms / 60);
  auto next = std::chrono::system_clock::now();
  renderContext.GameLoop([&]() {
    controller.SetControllerBitBasedOnInput(0);
    controller.SetControllerBitBasedOnInput(1);

    romWindow.Draw();
    RenderCompleteFrame(bus, renderContext);

    std::this_thread::sleep_until(next);
    next += diff;
  });
  return 0;
}
