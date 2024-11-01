#include "AudioDevice.h"
#include "Miniaudio.h"
#include "RingBuffer.h"
#include <iostream>

class AudioDevice::Impl {
 public:
  Impl();
  ~Impl();
  void Write(double data);
  double Read();
  void Start();
  void Mute();
  void UnMute();

 private:
  RingBuffer mRingBuffer;
  ma_device mDevice;
  double mMinReceivedSample = 0;
  double mMaxReceivedSample = 0.1;
  bool mMuted;

  void UpdateMinSample(double value);
  void UpdateMaxSample(double value);
  static void AudioCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount);
};

double Normalize(double value, double min, double max, double minDesired = -1.0, double maxDesired = 1.0) {
  const auto firstPart = (value - min) / (max - min);
  const auto range = maxDesired - minDesired;
  return firstPart * range + minDesired;
}

void AudioDevice::Impl::AudioCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount) {
  auto* asFloatPointer = reinterpret_cast<float*>(output);
  auto* audioDeviceImpl = reinterpret_cast<AudioDevice::Impl*>(device->pUserData);
  RingBuffer* ringBuffer = &audioDeviceImpl->mRingBuffer;
  for (int i = 0; i < frameCount; i++) {
    auto value = ringBuffer->Read();
    if (audioDeviceImpl->mMuted) {
      asFloatPointer[i] = 0.0;
    } else {
      audioDeviceImpl->UpdateMinSample(value);
      audioDeviceImpl->UpdateMaxSample(value);
      asFloatPointer[i] = Normalize(value, audioDeviceImpl->mMinReceivedSample, audioDeviceImpl->mMaxReceivedSample);
    }
  }
};

AudioDevice::Impl::Impl() : mRingBuffer(5000), mMuted(false) {
  ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = DEVICE_FORMAT;
  deviceConfig.playback.channels = DEVICE_CHANNELS;
  deviceConfig.sampleRate = DEVICE_SAMPLE_RATE;
  deviceConfig.pUserData = reinterpret_cast<void*>(this);
  deviceConfig.dataCallback = AudioCallback;

  if (ma_device_init(nullptr, &deviceConfig, &mDevice) != MA_SUCCESS) {
    std::cout << "Failed to open playback device.\n";
  }

  std::cout << "Device Name: " << mDevice.playback.name << "\n";
}

AudioDevice::Impl::~Impl() {
  ma_device_stop(&mDevice);
  ma_device_uninit(&mDevice);
}

void AudioDevice::Impl::Write(double data) { mRingBuffer.Write(data); }

double AudioDevice::Impl::Read() { return mRingBuffer.Read(); }

void AudioDevice::Impl::Start() {
  if (ma_device_start(&mDevice) != MA_SUCCESS) {
    std::cout << "Failed to start playback device.\n";
  }
}

void AudioDevice::Impl::UnMute() { mMuted = false; }

void AudioDevice::Impl::Mute() { mMuted = true; }

void AudioDevice::Impl::UpdateMinSample(double value) { mMinReceivedSample = std::min(value, mMinReceivedSample); }

void AudioDevice::Impl::UpdateMaxSample(double value) { mMaxReceivedSample = std::max(value, mMaxReceivedSample); }

AudioDevice::AudioDevice() : mImpl(std::make_unique<Impl>()) {}

AudioDevice::~AudioDevice() = default;

void AudioDevice::Write(double data) { mImpl->Write(data); }

void AudioDevice::Start() { mImpl->Start(); }

void AudioDevice::Mute() { mImpl->Mute(); }

void AudioDevice::UnMute() { mImpl->UnMute(); }
