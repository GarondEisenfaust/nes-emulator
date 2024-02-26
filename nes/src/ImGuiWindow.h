#pragma once
#include <functional>

class ImGuiWindow {
 public:
  ImGuiWindow(const char* windowName);
  virtual void Draw();

 protected:
  const char* mWindowName;
  bool mShow;
  std::function<void()> mDrawingFunction;
};
