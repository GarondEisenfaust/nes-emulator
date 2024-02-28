#pragma once
#include "ImGuiWindow.h"
#include <string>
#include <vector>

class LoadRomWindow : public ImGuiWindow {
 public:
  LoadRomWindow(const std::string& windowName, const std::string& romDirectory);
  std::string mCurrentRomPath = "";

 private:
  const std::string mRomDirectory;
  std::vector<std::string> mRomPaths;
};
