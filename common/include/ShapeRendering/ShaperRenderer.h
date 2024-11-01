#pragma once
#include "IShape.h"
#include <memory>
#include <vector>

class ShaperRenderer {
 public:
 private:
  std::vector<std::shared_ptr<IShape>> mShapes;
};