#include "rendering/VertexArray.h"

VertexArray::VertexArray() { glGenVertexArrays(1, &mHandle); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &mHandle); }

void VertexArray::Attach(std::shared_ptr<IBuffer> buffer) { mBuffers.push_back(buffer); }

void VertexArray::Upload() {
  Bind();
  for (auto buffer : mBuffers) {
    buffer->Upload();
  }
  Unbind();
}

void VertexArray::Bind() { glBindVertexArray(mHandle); }

void VertexArray::Unbind() { glBindVertexArray(0); }

void VertexArray::Draw() {
  Bind();
  glDrawArrays(GL_TRIANGLES, 0, mDrawBuffer->GetSize());
  Unbind();
}

void VertexArray ::SetDrawBuffer(std::shared_ptr<IBuffer> drawBuffer) { mDrawBuffer = drawBuffer; }
