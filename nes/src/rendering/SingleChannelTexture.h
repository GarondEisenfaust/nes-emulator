#pragma once
#include <stddef.h> 
#include <cstdint>

class SingleChannelTexture {
 public:
  SingleChannelTexture(size_t width, size_t height, int samplingMode, uint16_t* data = nullptr);
  ~SingleChannelTexture();
  void UpdateData(uint16_t* data);
  void Bind() const;

 private:
  unsigned int mTexture;
  size_t mWidth;
  size_t mHeight;
};
