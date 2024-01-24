#include "GL/glew.h"
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
  auto uniformLocation = glGetUniformLocation(mHandle, name);
  glUniform1f(uniformLocation, value);
}
