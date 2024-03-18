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

  void Write(double data);
  double Read();

 private:
  double* mStart;
  double* mEnd;

  double* writePointer;
  double* readPointer;

  double mLastRead;
  std::mutex mGuard;
  std::condition_variable condition;
  size_t mUnreadCount;
  size_t mSize;
  double* Advance(double* current);
};