#include "GL/glew.h"
#include "rendering/Texture.h"
#include <iostream>

Texture::Texture(size_t width, size_t height, int samplingMode, uint8_t* data) : mWidth(width), mHeight(height) {
  glGenTextures(1, &mHandle);
  Bind();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMode);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 240, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

Texture::~Texture() { glDeleteTextures(1, &mHandle); }

void Texture::UpdateData(uint8_t* data) {
  Bind();
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Texture::Bind() const { glBindTexture(GL_TEXTURE_2D, mHandle); }

unsigned int Texture::GetHandle() const { return mHandle; }
