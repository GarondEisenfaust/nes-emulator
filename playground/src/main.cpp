#define MA_NO_DECODING
#define MA_NO_ENCODING
#define MINIAUDIO_IMPLEMENTATION
#include "CircularBuffer.h"
#include "PulseChannel.h"
#include "miniaudio.h"
#include <chrono>
#include <cmath>
#include <queue>
#include <stdio.h>
#include <thread>
#include <vector>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>

void main_loop__em() {}
#endif

#define DEVICE_FORMAT ma_format_f32
#define DEVICE_CHANNELS 1
#define DEVICE_SAMPLE_RATE 44100

PulseChannel pulseChannel;
// CircularBuffer<float, 4 * 44100> circularBuffer;
std::queue<float> queue;
auto frameFinished = false;
float sineIdx = 0.0f;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
  for (int i = 0; i < frameCount; i++) {
    if (queue.size() <= 0) {
      frameCount = i;
      break;
    }
    auto u = queue.front();
    queue.pop();
    reinterpret_cast<float*>(pOutput)[i] = static_cast<float>(u);
  }
  frameFinished = false;
}

int main(int argc, char** argv) {
  ma_waveform sineWave;
  ma_device_config deviceConfig;
  ma_device device;
  ma_waveform_config sineWaveConfig;

  deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = DEVICE_FORMAT;
  deviceConfig.playback.channels = DEVICE_CHANNELS;
  deviceConfig.sampleRate = DEVICE_SAMPLE_RATE;
  deviceConfig.dataCallback = data_callback;
  deviceConfig.pUserData = &sineWave;

  if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
    printf("Failed to open playback device.\n");
    return -4;
  }

  printf("Device Name: %s\n", device.playback.name);

  sineWaveConfig = ma_waveform_config_init(device.playback.format, device.playback.channels, device.sampleRate,
                                           ma_waveform_type_square, 0.2, 220);
  ma_waveform_init(&sineWaveConfig, &sineWave);

  if (ma_device_start(&device) != MA_SUCCESS) {
    printf("Failed to start playback device.\n");
    ma_device_uninit(&device);
    return -5;
  }

  pulseChannel.mSequencer.sequence = 0b01111000;
  pulseChannel.mOscilator.dutycycle = 0.500;
  pulseChannel.mSequencer.enabled = true;
  pulseChannel.mEnvelope.startFlag = true;
  pulseChannel.mEnvelope.volume = 10;
  pulseChannel.mSequencer.reload = 15;
  pulseChannel.mSequencer.timer = 15;
  pulseChannel.mEnvelope.divider.SetPeriod(150000);
  pulseChannel.mOscilator.frequency = DEVICE_SAMPLE_RATE / 440;

  using namespace std::chrono_literals;
  auto diff = (1000ms / 60);
  auto clock = 0;
  while (true) {
    static auto next = std::chrono::system_clock::now();
    for (int i = 0; i < 14834; i++) {
      pulseChannel.Clock();
      std::cout << std::to_string(pulseChannel.output) << "\n";
      queue.push(pulseChannel.output);
      // circularBuffer.Write(pulseChannel.output);
    }
    clock++;
    frameFinished = true;
    std::this_thread::sleep_until(next);
    next += diff;
  }

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(main_loop__em, 0, 1);
#else
  printf("Press Enter to quit...\n");
  getchar();
#endif

  ma_device_uninit(&device);

  (void)argc;
  (void)argv;
  return 0;
}