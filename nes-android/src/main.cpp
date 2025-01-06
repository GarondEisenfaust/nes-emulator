#include "AndroidOut.h"
#include "Apu.h"
#include "ApuClockContext.h"
#include "AudioDevice.h"
#include "BackgroundRenderer.h"
#include "Bus.h"
#include "Controller.h"
#include "Cpu.h"
#include "ForegroundRenderer.h"
#include "IFrameDecoder.h"
#include "IRenderer.h"
#include "LookupTableFrameDecoder.h"
#include "LookupTableFrameDecoderGpu.h"
#include "NtscSignalFrameDecoder.h"
#include "NtscSignalFrameDecoderGpu.h"
#include "Ppu.h"
#include "RenderContext.h"
#include "ShapeRendering/SolidRectangleRenderer.h"
#include <game-activity/GameActivity.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <game-text-input/gametextinput.h>
#include <chrono>
#include <cstdio>
#include <jni.h>
#include <thread>
#include <unistd.h>

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

std::unique_ptr<RenderContext> renderContext;
std::unique_ptr<IFrameDecoder> decoder;
std::unique_ptr<Ram> ram;
std::unique_ptr<Bus> bus;
std::unique_ptr<Cpu> cpu;
std::unique_ptr<Ppu> ppu;
std::unique_ptr<Apu> apu;
std::unique_ptr<ForegroundRenderer> foregroundRenderer;
std::unique_ptr<BackgroundRenderer> backgroundRenderer;
std::unique_ptr<Controller> controller;
std::unique_ptr<AudioDevice> audioDevice;
std::unique_ptr<SolidRectangleRenderer> rectangleRenderer;
std::unique_ptr<ApuClockContext> apuClockContext;

bool initialized = false;

bool InitNes(android_app* pApp) {
  renderContext = std::make_unique<RenderContext>();
  renderContext->Init(pApp);

  decoder = std::make_unique<NtscSignalFrameDecoderGpu>(renderContext->GetWidth(), renderContext->GetHeight());
  renderContext->SetFrameDecoder(decoder.get());

  ram = std::make_unique<Ram>();
  bus = std::make_unique<Bus>(*ram);
  cpu = std::make_unique<Cpu>();
  ppu = std::make_unique<Ppu>(*renderContext);
  apu = std::make_unique<Apu>();
  audioDevice = std::make_unique<AudioDevice>();

  foregroundRenderer = std::make_unique<ForegroundRenderer>();
  foregroundRenderer->SetPpu(ppu.get());
  ppu->SetForegroundRenderer(foregroundRenderer.get());

  backgroundRenderer = std::make_unique<BackgroundRenderer>();
  backgroundRenderer->SetPpu(ppu.get());
  ppu->SetBackgroundRenderer(backgroundRenderer.get());
  rectangleRenderer = std::make_unique<SolidRectangleRenderer>(renderContext->GetWidth(), renderContext->GetHeight());

  controller = std::make_unique<Controller>(*rectangleRenderer);

  bus->ConnectController(controller.get());
  cpu->ConnectBus(bus.get());
  ppu->ConnectBus(bus.get());
  apu->ConnectBus(bus.get());
  using namespace std::chrono_literals;

  bus->InsertCartridge(std::make_shared<Cartridge>(theRomFd));
  bus->Reset();

  apuClockContext = std::make_unique<ApuClockContext>(*apu, *audioDevice);
  audioDevice->Start();

  return true;
}

void handle_cmd(android_app* pApp, int32_t cmd) {
  switch (cmd) {
    case APP_CMD_INIT_WINDOW:
      // A new window is created, associate a renderer with it. You may replace this with a
      // "game" class if that suits your needs. Remember to change all instances of userData
      // if you change the class here as a reinterpret_cast is dangerous this in the
      // android_main function and the APP_CMD_TERM_WINDOW handler case.
      initialized = InitNes(pApp);
      break;
    default:
      break;
  }
}

extern "C" void android_main(struct android_app* app) {
  app->onAppCmd = handle_cmd;

  int events;
  struct android_poll_source* source;

  using namespace std::chrono_literals;
  const auto diff = (1000ms / 60);
  auto next = std::chrono::system_clock::now();

  while (true) {
    while ((ALooper_pollAll(0, nullptr, &events, (void**)&source)) >= 0) {
      if (source) {
        source->process(source->app, source);
      }
      if (app->destroyRequested) {
        return;
      }
    }

    if (!initialized) {
      continue;
    }

    renderContext->DrawOneFrame([&]() {
      controller->HandleInputEvents(&app->inputBuffers[app->currentInputBuffer]);
      RenderCompleteFrame(*bus, *renderContext);
      rectangleRenderer->Render();
      std::this_thread::sleep_until(next);
      next += diff;
    });
  }
}

extern "C" JNIEXPORT void JNICALL Java_com_example_nes_1emulator_NesActivity_setRomFd(JNIEnv* env, jobject obj,
                                                                                      int fd) {
  theRomFd = fd;
}
