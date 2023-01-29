#pragma once
#include <functional>

class GuiWindow {
 public:
  GuiWindow(const char* title, bool open = true);
  ~GuiWindow() = default;

  void Render();
  virtual void Content();

 private:
  const char* mTitle;
  bool mOpen;
};