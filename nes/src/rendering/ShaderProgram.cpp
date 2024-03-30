#include <glad/glad.h>
#include "rendering/ShaderProgram.h"

ShaderProgram::ShaderProgram() { mHandle = glCreateProgram(); }

ShaderProgram::~ShaderProgram() {
  glUseProgram(0);
  glDeleteProgram(mHandle);
}

void ShaderProgram::AttachShader(const Shader& shader) { glAttachShader(mHandle, shader.GetHandle()); }

void ShaderProgram::Link() { glLinkProgram(mHandle); }

void ShaderProgram::Use() { glUseProgram(mHandle); }

void ShaderProgram::SetUniform(const char* name, float value) {
  const auto uniformLocation = glGetUniformLocation(mHandle, name);
  glUniform1f(uniformLocation, value);
}

void ShaderProgram::SetUniform(const char* name, int value) {
  const auto uniformLocation = glGetUniformLocation(mHandle, name);
  glUniform1i(uniformLocation, value);
}

void ShaderProgram::SetUniform(const char* name, const ShortTexture& texture) {
  const auto uniformLocation = glGetUniformLocation(mHandle, name);
  glUniform1i(uniformLocation, texture.GetHandle());
}
