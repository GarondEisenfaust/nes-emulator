#pragma once
#include <array>

template <typename TYPE, int SIZE>
class CircularBuffer {
 public:
  CircularBuffer() : mBuffer({0}), mReadIndex(0), mWriteIndex(0) {}

  void Write(TYPE value) {
    mBuffer[mWriteIndex] = value;
    IncrementIndex(mWriteIndex);
  }

  TYPE Read() {
    auto value = mBuffer[mReadIndex];
    if (mReadIndex < mWriteIndex) {
      IncrementIndex(mReadIndex);
    }
    return value;
  }

 private:
  std::array<TYPE, SIZE> mBuffer;
  int mWriteIndex;
  int mReadIndex;

  void IncrementIndex(int& index) { index = (index + 1) % mBuffer.size(); }
};
