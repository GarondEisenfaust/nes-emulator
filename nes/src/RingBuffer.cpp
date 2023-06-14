#include "RingBuffer.h"
#include <cstring>
#include <vector>

RingBuffer::RingBuffer() {}

void RingBuffer::Write(float data) { mQueue.push(data); }

float RingBuffer::Read() {
  if (mQueue.empty()) {
    return 0;
  }
  auto temp = mQueue.front();
  mQueue.pop();
  return temp;
}
