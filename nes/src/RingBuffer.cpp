#include "RingBuffer.h"

RingBuffer::RingBuffer(int size) : mSize(size) {
  mStart = new float[mSize]{0};
  mEnd = mStart + size;
  readPointer = mStart;
  writePointer = mEnd;
}

RingBuffer::~RingBuffer() { delete[] mStart; }

void RingBuffer::Write(float data) {
  if (mUnreadCount > mSize) {
    return;
  }

  *writePointer = data;
  mUnreadCount++;
  writePointer = Advance(writePointer);
}

float RingBuffer::Read() {
  if (mUnreadCount > 0) {
    mLastRead = *readPointer;
    mUnreadCount--;
    readPointer = Advance(readPointer);
  }
  return mLastRead;
}

float* RingBuffer::Advance(float* current) {
  if (current == mEnd) {
    return mStart;
  }
  return ++current;
}
