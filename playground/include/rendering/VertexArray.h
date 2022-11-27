#pragma once
#include "IBuffer.h"
#include "OpenglObject.h"
#include <GL/glew.h>
#include <memory>
#include <vector>

class VertexArray : public OpenglObject {
 public:
  VertexArray();
  ~VertexArray();

  void Attach(std::shared_ptr<IBuffer> buffer);
  void Upload();
  void Bind();
  void Unbind();
  void Draw();
  void SetDrawBuffer(std::shared_ptr<IBuffer> drawBuffer);

 private:
  std::vector<std::shared_ptr<IBuffer>> mBuffers;
  std::shared_ptr<IBuffer> mDrawBuffer;
};
