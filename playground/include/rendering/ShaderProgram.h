#pragma once
#include "Shader.h"
#include <GL/glew.h>
#include <memory>

class ShaderProgram {
 public:
  ShaderProgram();
  void AttachShader(const Shader& shader);
  GLuint GetHandle();
  void Link();
  void Use();

 private:
  std::unique_ptr<GLuint> mHandle;
};
