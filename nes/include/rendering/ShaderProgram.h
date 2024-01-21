#pragma once
#include "Shader.h"
#include <GL/glew.h>
#include <memory>

class ShaderProgram {
 public:
  ShaderProgram();
  ~ShaderProgram();
  void AttachShader(const Shader& shader);
  GLuint GetHandle();
  void Link();
  void Use();
  void SetUniform(const char* name, float value);

 private:
  std::unique_ptr<GLuint> mHandle;
};
