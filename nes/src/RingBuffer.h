#pragma once
#include "IAudioOutputDevice.h"
#include <queue>

class RingBuffer : public IAudioOutputDevice {
 public:
  RingBuffer();
  ~RingBuffer() = default;
  void Write(float data) override;
  float Read() override;

 private:
  std::queue<float> mQueue;
};