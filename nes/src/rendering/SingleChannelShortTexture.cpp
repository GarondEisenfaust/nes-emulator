#include <glad/glad.h>
#include "rendering/SingleChannelShortTexture.h"
#include <iostream>

SingleChannelShortTexture::SingleChannelShortTexture(size_t width, size_t height, int samplingMode, uint16_t* data)
    : mWidth(width), mHeight(height) {
  glGenTextures(1, &mHandle);
  Bind();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMode);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R16UI, mWidth, mHeight, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, data);
}

SingleChannelShortTexture::~SingleChannelShortTexture() { glDeleteTextures(1, &mHandle); }

void SingleChannelShortTexture::UpdateData(uint16_t* data) {
  Bind();
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, GL_RED_INTEGER, GL_UNSIGNED_SHORT, data);
}

void SingleChannelShortTexture::Bind() const { glBindTexture(GL_TEXTURE_2D, mHandle); }

void SingleChannelShortTexture::Bind(unsigned int textureUnit) const {
  glActiveTexture(GL_TEXTURE0 + textureUnit);
  Bind();
}

unsigned int SingleChannelShortTexture::GetHandle() const { return mHandle; }
