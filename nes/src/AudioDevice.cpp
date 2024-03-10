#include "AudioDevice.h"
#include "Miniaudio.h"
#include "RingBuffer.h"
#include <iostream>

class AudioDevice::Impl {
 public:
  Impl();
  ~Impl();
  void Write(float data);
  float Read();
  void Mute();
  void UnMute();

  void UpdateMinSample(float value);
  void UpdateMaxSample(float value);

 private:
  RingBuffer mRingBuffer;
  ma_device mDevice;
  float mMinReceivedSample = 0;
  float mMaxReceivedSample = 0.1;
  bool mMuted;
  static void AudioCallback(ma_device* device, void* output, const void* input, ma_uint32 frameCount);
};

float Normalize(float value, float min, float max, float minDesired = -1, float maxDesired = 1) {
  auto firstPart = (value - min) / (max - min);
  auto range = maxDesired - minDesired;
  return firstPart * range + minDesired;
}

void AudioDevice::Impl::AudioCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
  auto* asFloatPointer = reinterpret_cast<float*>(pOutput);
  auto* audioDeviceImpl = reinterpret_cast<AudioDevice::Impl*>(pDevice->pUserData);
  RingBuffer* ringBuffer = &audioDeviceImpl->mRingBuffer;
  for (int i = 0; i < frameCount; i++) {
    auto value = ringBuffer->Read();
    if (audioDeviceImpl->mMuted) {
      asFloatPointer[i] = 0;
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

  if (ma_device_init(NULL, &deviceConfig, &mDevice) != MA_SUCCESS) {
    std::cout << "Failed to open playback device.\n";
  }

  std::cout << "Device Name: " << mDevice.playback.name << "\n";

  if (ma_device_start(&mDevice) != MA_SUCCESS) {
    std::cout << "Failed to start playback device.\n";
  }
}

AudioDevice::Impl::~Impl() {
  ma_device_stop(&mDevice);
  ma_device_uninit(&mDevice);
}

void AudioDevice::Impl::Write(float data) { mRingBuffer.Write(data); }

float AudioDevice::Impl::Read() { return mRingBuffer.Read(); }

void AudioDevice::Impl::UnMute() { mMuted = false; }

void AudioDevice::Impl::Mute() { mMuted = true; }

void AudioDevice::Impl::UpdateMinSample(float value) { mMinReceivedSample = std::min(value, mMinReceivedSample); }

void AudioDevice::Impl::UpdateMaxSample(float value) { mMaxReceivedSample = std::max(value, mMaxReceivedSample); }

AudioDevice::AudioDevice() : mImpl(std::make_unique<Impl>()) {}

AudioDevice::~AudioDevice() = default;

void AudioDevice::Write(float data) { mImpl->Write(data); }

float AudioDevice::Read() { return mImpl->Read(); }

void AudioDevice::Mute() { mImpl->Mute(); }

void AudioDevice::UnMute() { mImpl->UnMute(); }
