#pragma once
#include "IBuffer.h"
#include "OpenglObject.h"
#include "Util.h"
#include "VertexArray.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

template <typename TYPE>
class Buffer : public IBuffer, public OpenglObject {
 public:
  Buffer(GLuint index, GLuint size, GLuint drawMode) : mIndex(index), mSize(size), mDrawMode(drawMode) {
    glGenBuffers(1, &mHandle);
  }

  ~Buffer() { glDeleteBuffers(1, &mHandle); }

  void Upload() const override {
    glBindBuffer(GL_ARRAY_BUFFER, mHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TYPE) * mBuffer.size(), mBuffer.data(), mDrawMode);

    glVertexAttribPointer(mIndex, mSize, GetGlEnumByType<TYPE>(), GL_FALSE, 0, 0);
    glEnableVertexAttribArray(mIndex);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void Load(const std::vector<TYPE>& data) { mBuffer = data; }
  void Attach(VertexArray& vertexArray) const { vertexArray.Attach(this); }
  void SetData(std::vector<TYPE>& buffer) { mBuffer = buffer; }
  GLuint GetSize() const override { return mBuffer.size(); }
  GLuint GetIndex() const override { return mIndex; }

 private:
  std::vector<TYPE> mBuffer;
  GLuint mDrawMode;
  GLuint mSize;
  GLuint mIndex;
};
