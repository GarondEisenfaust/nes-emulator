#pragma once

class Surface {
 public:
  Surface();
  ~Surface();
  void Draw();

 private:
  unsigned int mVao;
  unsigned int mVbo;
  float mVertices[20] = {
      -1, -1, 0, 0, 1, 1, -1, 0, 1, 1, 1, 1, 0, 1, 0, -1, 1, 0, 0, 0,
  };
};
