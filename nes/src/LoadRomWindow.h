#pragma once
#include <string>
#include <vector>

class Bus;
class LoadRomWindow {
 public:
  LoadRomWindow(const std::string& windowName, const std::string& romDirectory, Bus* bus);
  void SetBus(Bus* bus);
  void Draw();

 private:
  Bus* mBus;
  bool mShow;
  bool mStartCollapsed;
  const std::string mRomDirectory;
  const std::string mWindowName;
  std::vector<std::string> mRomPaths;
  float xPos = 0;
  float yPos = 0;
  void DrawButtons();
};
