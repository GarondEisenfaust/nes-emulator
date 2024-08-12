#pragma once
#include "ColorPalette.h"
#include "Shader.h"
#include "ShortTexture.h"
#include <memory>

class ShaderProgram {
 public:
  ShaderProgram();
  ~ShaderProgram();
  void AttachShader(const Shader& shader);
  void Link();
  void Use();

  void SetUniform(const char* name, ColorPaletteF& colorPalette);
  void SetUniform(const char* name, float value);
  void SetUniform(const char* name, int value);
  void SetUniform(const char* name, const ShortTexture& texture);

 private:
  unsigned int mHandle;
};
