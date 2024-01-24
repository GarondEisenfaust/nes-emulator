#pragma once
#include "Shader.h"
#include <memory>

class ShaderProgram {
 public:
  ShaderProgram();
  ~ShaderProgram();
  void AttachShader(const Shader& shader);
  void Link();
  void Use();
  void SetUniform(const char* name, float value);

 private:
  unsigned int mHandle;
};
