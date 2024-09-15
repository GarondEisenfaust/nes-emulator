#include "ShapeRendering/ShaderProgram.h"
#include "../OpenGL.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ColorPalette.h>

ShaderProgram::ShaderProgram() { mHandle = glCreateProgram(); }

ShaderProgram::~ShaderProgram() {
  glUseProgram(0);
  glDeleteProgram(mHandle);
}

void ShaderProgram::AttachShader(const Shader& shader) { glAttachShader(mHandle, shader.GetHandle()); }

void ShaderProgram::Link() { glLinkProgram(mHandle); }

void ShaderProgram::Use() { glUseProgram(mHandle); }

void ShaderProgram::SetUniform(const char* name, ColorPaletteF& colorPalette) {
  const auto uniformLocation = glGetUniformLocation(mHandle, name);
  glUniform4fv(uniformLocation, colorPalette.size(), reinterpret_cast<float*>(colorPalette.data()));
}

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

void ShaderProgram::SetUniform(const char* name, const glm::mat4& matrix) {
  const auto uniformLocation = glGetUniformLocation(mHandle, name);
  glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::SetUniform(const char* name, const glm::vec4& vector) {
  const auto uniformLocation = glGetUniformLocation(mHandle, name);
  glUniform4fv(uniformLocation, 1, glm::value_ptr(vector));
}

void ShaderProgram::SetUniform(const char* name, const PixelColorF& color) {
  SetUniform(name, glm::vec4{color.r, color.g, color.b, color.a});
}
