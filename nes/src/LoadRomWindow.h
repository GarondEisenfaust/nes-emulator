#pragma once
#include "ImGuiWindow.h"
#include <string>
#include <vector>

class LoadRomWindow : public ImGuiWindow {
 public:
  LoadRomWindow(const std::string& windowName, const std::string& romDirectory);
  void Draw() override;
  std::string mCurrentRomPath = "";

 private:
  bool mRomDirectoryInitialized;
  const std::string mRomDirectory;
  std::vector<std::string> mRomPaths;
};
