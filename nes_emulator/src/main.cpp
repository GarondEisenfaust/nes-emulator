#include "AndroidOut.h"
#include "Apu.h"
#include "AudioDevice.h"
#include "BackgroundRenderer.h"
#include "Bus.h"
#include "Controller.h"
#include "Cpu.h"
#include "ForegroundRenderer.h"
#include "IRenderer.h"
#include "Ppu.h"
#include "rendering/IFrameDecoder.h"
#include "rendering/LookupTableFrameDecoder.h"
#include "rendering/LookupTableFrameDecoderGpu.h"
#include "rendering/NtscSignalFrameDecoder.h"
#include "rendering/NtscSignalFrameDecoderGpu.h"
#include "rendering/RenderContext.h"
#include <game-activity/GameActivity.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <game-text-input/gametextinput.h>
#include <chrono>
#include <cstdio>
#include <jni.h>
#include <thread>
#include <unistd.h>

std::string theRomPath = "";
int theRomFd = 0;

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
RenderContext renderContext;

std::unique_ptr<IFrameDecoder> CreateDecoder() { return std::make_unique<NtscSignalFrameDecoderGpu>(); }
extern "C" {

void handle_cmd(android_app* pApp, int32_t cmd) {
  switch (cmd) {
    case APP_CMD_INIT_WINDOW:
      // A new window is created, associate a renderer with it. You may replace this with a
      // "game" class if that suits your needs. Remember to change all instances of userData
      // if you change the class here as a reinterpret_cast is dangerous this in the
      // android_main function and the APP_CMD_TERM_WINDOW handler case.
      renderContext.Init(pApp);
      break;
    default:
      break;
  }
}

void android_main(struct android_app* app) {
  app->onAppCmd = handle_cmd;

  std::unique_ptr<IFrameDecoder> decoder = CreateDecoder();
  renderContext.SetFrameDecoder(decoder.get());

  auto ram = std::make_unique<Ram>();
  Bus bus(*ram);
  Cpu cpu;
  Ppu ppu(renderContext);
  Apu apu;

  ForegroundRenderer foregroundRenderer;
  foregroundRenderer.SetPpu(&ppu);
  ppu.SetForegroundRenderer(&foregroundRenderer);

  BackgroundRenderer backgroundRenderer;
  backgroundRenderer.SetPpu(&ppu);
  ppu.SetBackgroundRenderer(&backgroundRenderer);

  Controller controller;

  bus.ConnectController(&controller);
  cpu.ConnectBus(&bus);
  ppu.ConnectBus(&bus);
  apu.ConnectBus(&bus);
  using namespace std::chrono_literals;

  bus.InsertCartridge(std::make_shared<Cartridge>(theRomFd));
  bus.Reset();

  AudioDevice audioDevice;

  while(!renderContext.mInitialized) { std::this_thread::sleep_for(1s); }

  const auto diff = (1000ms / 60);
  auto next = std::chrono::system_clock::now();
  renderContext.GameLoop([&]() {
    RenderCompleteFrame(bus, renderContext);
    std::this_thread::sleep_until(next);
    next += diff;
  });
}
}

extern "C" JNIEXPORT void JNICALL Java_com_example_nes_1emulator_NesActivity_setRomFd(JNIEnv* env, jobject obj, int fd) {
  theRomFd = fd;
}
