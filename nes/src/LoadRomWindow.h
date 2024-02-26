#pragma once
#include "ImGuiWindow.h"
#include <string>
#include <vector>

class LoadRomWindow : public ImGuiWindow {
 public:
  LoadRomWindow(const char* windowName, const char* romDirectory);
  std::string mCurrentRomPath = "";

 private:
  const char* mRomDirectory;
  std::vector<std::string> mRomPaths;
};
