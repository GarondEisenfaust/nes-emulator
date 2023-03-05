#pragma once
#include "GL/glew.h"
#include <string>
#include <string_view>
#include <tuple>

class Shader {
 public:
  Shader(const char* code, GLuint shaderType);
  ~Shader();
  GLuint GetHandle() const;
  bool WasSuccessful() const;
  void Delete();

 private:
  GLuint mHandle;
  bool mSuccessful;
  std::string mShaderCode;
  GLuint mShaderType;

  static std::tuple<GLint, GLuint> Compile(const GLchar* shaderProgram, GLuint shaderType);
};
