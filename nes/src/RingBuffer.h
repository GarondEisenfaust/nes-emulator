#pragma once
#include "IAudioOutputDevice.h"

class RingBuffer : public IAudioOutputDevice {
 public:
  RingBuffer(int size);
  ~RingBuffer();
  void Write(float data) override;
  float Read() override;
  float* Advance(float* current);

 private:
  float* mStart;
  float* mEnd;

  float* writePointer;
  float* readPointer;
};