#include "GL/glew.h"
#include "Texture.h"
#include <iostream>

Texture::Texture(size_t width, size_t height, int samplingMode, uint16_t* data) : mWidth(width), mHeight(height) {
  glGenTextures(1, &mTexture);
  Bind();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, samplingMode);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, samplingMode);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, mWidth, mHeight, 0, GL_RED_INTEGER, GL_UNSIGNED_SHORT, data);
  // glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture() { glDeleteTextures(1, &mTexture); }

void Texture::UpdateData(uint16_t* data) {
  Bind();
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, GL_RED_INTEGER, GL_UNSIGNED_SHORT, data);
}

void Texture::Bind() { glBindTexture(GL_TEXTURE_2D, mTexture); }
