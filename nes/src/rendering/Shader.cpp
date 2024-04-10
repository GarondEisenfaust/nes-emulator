#include <glad/glad.h>
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

Shader::Shader(const char* code, unsigned int shaderType) {
  mShaderType = shaderType;
  auto [success, handle] = Compile(code, mShaderType);
  mSuccessful = success;
  mHandle = handle;
}

Shader::~Shader() { Delete(); }

unsigned int Shader::GetHandle() const { return mHandle; }

bool Shader::WasSuccessful() const { return mSuccessful; }

std::tuple<int, unsigned int> Shader::Compile(const char* shaderProgram, unsigned int shaderType) {
  unsigned int handle = glCreateShader(shaderType);
  glShaderSource(handle, 1, &shaderProgram, nullptr);
  glCompileShader(handle);

  int success;
  glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

  int logLength;
  glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);
  std::vector<char> vertShaderError((logLength > 1) ? logLength : 1);
  glGetShaderInfoLog(handle, logLength, nullptr, &vertShaderError[0]);
  std::cout << &vertShaderError[0] << std::endl;

  return {success, handle};
}

void Shader::Delete() { glDeleteShader(mHandle); }
