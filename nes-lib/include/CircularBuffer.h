#pragma once
#include <array>

template <typename TYPE, int SIZE>
class CircularBuffer {
 public:
  CircularBuffer() : mBuffer({0}), mWritePointer(mBuffer.begin()), mReadPointer(mBuffer.begin()) {}

  void Write(TYPE value) {
    *mWritePointer = value;
    if (value != 0) {
      auto p = 0;
    }
    mWritePointer = NextPointer(mWritePointer);
  }

  TYPE Read() {
    auto value = *mWritePointer;
    mWritePointer = NextPointer(mWritePointer);
    return value;
  }

 private:
  std::array<TYPE, SIZE> mBuffer;
  TYPE* mWritePointer;
  TYPE* mReadPointer;

  TYPE* NextPointer(TYPE* pointer) {
    pointer++;
    if (pointer == mBuffer.end()) {
      pointer = mBuffer.begin();
    }
    return pointer;
  }
};
