#pragma once
#include <functional>
#include <string>

class ImGuiWindow {
 public:
  ImGuiWindow(const std::string& windowName);
  virtual void Draw();

 protected:
  const std::string mWindowName;
  bool mShow;
  std::function<void()> mDrawingFunction;
};
