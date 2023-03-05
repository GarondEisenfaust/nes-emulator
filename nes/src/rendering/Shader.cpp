#include "rendering/Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

std::string ReadFile(std::string_view path) {
  std::ifstream fileStream(path.begin());
  std::stringstream buffer;
  buffer << fileStream.rdbuf();
  return buffer.str();
}

Shader::Shader(const char* code, GLuint shaderType) {
  mShaderType = shaderType;
  auto [success, handle] = Compile(code, mShaderType);
  mSuccessful = success;
  mHandle = handle;
}

Shader::~Shader() { Delete(); }

GLuint Shader::GetHandle() const { return mHandle; }

bool Shader::WasSuccessful() const { return mSuccessful; }

std::tuple<GLint, GLuint> Shader::Compile(const GLchar* shaderProgram, GLuint shaderType) {
  GLuint handle = glCreateShader(shaderType);
  glShaderSource(handle, 1, &shaderProgram, NULL);
  glCompileShader(handle);

  GLint success;
  glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

  int logLength;
  glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
  std::vector<char> vertShaderError((logLength > 1) ? logLength : 1);
  glGetShaderInfoLog(handle, logLength, NULL, &vertShaderError[0]);
  std::cout << &vertShaderError[0] << std::endl;

  return {success, handle};
}

void Shader::Delete() { glDeleteShader(mHandle); }
