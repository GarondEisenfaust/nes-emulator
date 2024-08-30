#pragma once
#include <string>
#include <string_view>
#include <tuple>

class Shader {
 public:
  Shader(const char* code, unsigned int shaderType);
  ~Shader();
  unsigned int GetHandle() const;
  bool WasSuccessful() const;
  void Delete();

 private:
  unsigned int mHandle;
  bool mSuccessful;
  std::string mShaderCode;
  unsigned int mShaderType;

  static std::tuple<int, unsigned int> Compile(const char* shaderProgram, unsigned int shaderType);
};
