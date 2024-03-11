#include "RingBuffer.h"

RingBuffer::RingBuffer(int size) : mSize(size), mUnreadCount(0) {
  mStart = new float[mSize]{0};
  mEnd = mStart + size;
  readPointer = mStart;
  writePointer = mEnd;
}

RingBuffer::~RingBuffer() { delete[] mStart; }

void RingBuffer::Write(float data) {
  std::unique_lock<std::mutex> lock(mGuard);
  condition.wait(lock, [&]() { return mUnreadCount < mSize; });
  *writePointer = data;
  writePointer = Advance(writePointer);
  mUnreadCount++;
}

float RingBuffer::Read() {
  std::unique_lock<std::mutex> lock(mGuard);
  if (mUnreadCount > 0) {
    mLastRead = *readPointer;
    readPointer = Advance(readPointer);
    mUnreadCount--;
  }
  condition.notify_one();
  return mLastRead;
}

float* RingBuffer::Advance(float* current) {
  if (current == mEnd) {
    return mStart;
  }
  return ++current;
}
