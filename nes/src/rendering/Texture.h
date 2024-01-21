#pragma once
#include <cstdint>

class Texture {
 public:
  Texture(size_t width, size_t height, int samplingMode, uint16_t* data = nullptr);
  ~Texture();
  void UpdateData(uint16_t* data);
  void Bind();

 private:
  unsigned int mTexture;
  size_t mWidth;
  size_t mHeight;
};
