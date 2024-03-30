#include <glad/glad.h>
#include "rendering/ShortTexture.h"
#include <iostream>

ShortTexture::ShortTexture(size_t width, size_t height, int samplingMode, uint16_t* data)
    : mWidth(width), mHeight(height) {
  glGenTextures(1, &mHandle);
  Bind();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMode);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16UI, mWidth, mHeight, 0, GL_RGBA_INTEGER, GL_UNSIGNED_SHORT, data);
}

ShortTexture::~ShortTexture() { glDeleteTextures(1, &mHandle); }

void ShortTexture::UpdateData(uint16_t* data) {
  Bind();
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, GL_RGBA_INTEGER, GL_UNSIGNED_SHORT, data);
}

void ShortTexture::Bind() const { glBindTexture(GL_TEXTURE_2D, mHandle); }

void ShortTexture::Bind(unsigned int textureUnit) const {
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  Bind();
}

unsigned int ShortTexture::GetHandle() const { return mHandle; }
