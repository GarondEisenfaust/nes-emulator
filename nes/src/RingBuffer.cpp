#include "RingBuffer.h"

RingBuffer::RingBuffer(int size) : mSize(size), mUnreadCount(0) {
  mStart = new double[mSize]{0};
  mEnd = mStart + size;
  readPointer = mStart;
  writePointer = mEnd;
}

RingBuffer::~RingBuffer() { delete[] mStart; }

void RingBuffer::Write(double data) {
  std::unique_lock<std::mutex> lock(mGuard);
  condition.wait(lock, [&]() { return mUnreadCount < mSize; });
  *writePointer = data;
  writePointer = Advance(writePointer);
  mUnreadCount++;
}

double RingBuffer::Read() {
  std::unique_lock<std::mutex> lock(mGuard);
  if (mUnreadCount > 0) {
    mLastRead = *readPointer;
    readPointer = Advance(readPointer);
    mUnreadCount--;
  }
  condition.notify_one();
  return mLastRead;
}

double* RingBuffer::Advance(double* current) {
  if (current == mEnd) {
    return mStart;
  }
  return ++current;
}
