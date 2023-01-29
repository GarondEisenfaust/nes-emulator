#include "rendering/ShaderProgram.h"

ShaderProgram::ShaderProgram() { mHandle.reset(); }

void ShaderProgram::AttachShader(const Shader& shader) { glAttachShader(GetHandle(), shader.GetHandle()); }

void ShaderProgram::Link() { glLinkProgram(GetHandle()); }

GLuint ShaderProgram::GetHandle() {
  if (!mHandle) {
    auto handle = glCreateProgram();
    mHandle = std::make_unique<GLuint>(handle);
  }
  return *mHandle;
}

void ShaderProgram::Use() { glUseProgram(GetHandle()); }

void ShaderProgram::SetUniform(const char* name, float value) {
  auto uniformLocation = glGetUniformLocation(GetHandle(), name);
  glUniform1f(uniformLocation, value);
}
