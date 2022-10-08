#pragma once
#include "GuiWindow.h"
#include "MemoryReader.h"

class MemoryWindow : public GuiWindow {
 public:
  MemoryWindow(const char* title, const MemoryReader& memoryReader);
  ~MemoryWindow() = default;
  void Content() override;

 private:
  const MemoryReader& mMemoryReader;
};
