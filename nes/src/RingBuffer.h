#pragma once
#include "IAudioOutputDevice.h"
#include <atomic>
#include <mutex>

class RingBuffer : public IAudioOutputDevice {
 public:
  RingBuffer(int size);
  ~RingBuffer();
  void Write(float data) override;
  float Read() override;
  float* Advance(float* current);

 private:
  int mSize;
  float* mStart;
  float* mEnd;

  float* writePointer;
  float* readPointer;

  float mLastRead;
  std::atomic<unsigned long> mUnreadCount;
};