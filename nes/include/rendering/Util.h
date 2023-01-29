#pragma once

#include <GL/glew.h>
#include <type_traits>

template <typename TYPE>
constexpr GLuint GetGlEnumByType() {
  if (std::is_same_v<TYPE, GLfloat>) {
    return GL_FLOAT;
  }
}
