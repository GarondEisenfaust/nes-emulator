#pragma once
#include "IAudioOutputDevice.h"
#include <condition_variable>
#include <mutex>

class RingBuffer {
 public:
  RingBuffer(int size);
  ~RingBuffer();

  RingBuffer(const RingBuffer&) = delete;
  RingBuffer(const RingBuffer&&) = delete;
  RingBuffer& operator=(const RingBuffer&) = delete;
  RingBuffer& operator=(RingBuffer&&) = delete;

  void Write(float data);
  float Read();

 private:
  float* mStart;
  float* mEnd;

  float* writePointer;
  float* readPointer;

  float mLastRead;
  std::mutex mGuard;
  std::condition_variable condition;
  size_t mUnreadCount;
  size_t mSize;
  float* Advance(float* current);
};