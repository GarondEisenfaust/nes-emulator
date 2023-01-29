#pragma once
#include "GL/glew.h"

class OpenglObject {
 public:
  GLuint GetHandle();

 protected:
  GLuint mHandle;
};