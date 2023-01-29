#pragma once
#include "GL/glew.h"
#include <vector>

class IBuffer {
 public:
  virtual void Upload() const = 0;
  virtual GLuint GetSize() const = 0;
  virtual GLuint GetIndex() const = 0;
};