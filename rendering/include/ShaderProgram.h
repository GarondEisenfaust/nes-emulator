#pragma once
#include "ColorPalette.h"
#include "Shader.h"
#include "ShortTexture.h"
#include <glm/mat4x4.hpp>
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
  void SetUniform(const char* name, const glm::mat4& matrix);

 private:
  unsigned int mHandle;
};
