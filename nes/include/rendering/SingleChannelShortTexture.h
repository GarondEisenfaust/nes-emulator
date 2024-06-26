#pragma once
#include <cstdint>
#include <stddef.h>

class SingleChannelShortTexture {
 public:
  SingleChannelShortTexture(size_t width, size_t height, int samplingMode, uint16_t* data = nullptr);
  ~SingleChannelShortTexture();
  void UpdateData(uint16_t* data);
  void Bind() const;
  void Bind(unsigned int textureUnit) const;
  unsigned int GetHandle() const;

 private:
  unsigned int mHandle;
  size_t mWidth;
  size_t mHeight;
};
