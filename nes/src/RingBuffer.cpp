#include "RingBuffer.h"
#include <cstring>
#include <vector>

RingBuffer::RingBuffer(int size) {
  mStart = new float[size]{0};
  mEnd = mStart + size;
  readPointer = mStart;
  writePointer = mEnd;
}

RingBuffer::~RingBuffer() { delete[] mStart; }

void RingBuffer::Write(float data) {
  *writePointer = data;
  writePointer = Advance(writePointer);
}

float RingBuffer::Read() {
  auto temp = *readPointer;
  readPointer = Advance(readPointer);
  return temp;
}

float* RingBuffer::Advance(float* current) {
  if (current == mEnd) {
    return mStart;
  }
  return ++current;
}
