#pragma once
#include <string>
#include <vector>

class Bus;
class LoadRomWindow {
 public:
  LoadRomWindow(const std::string& windowName, const std::string& romDirectory, Bus* bus);
  void SetBus(Bus* bus);
  bool Draw();

 private:
  Bus* mBus;
  bool mShow;
  const std::string mRomDirectory;
  const std::string mWindowName;
  std::vector<std::string> mRomPaths;
  int mFlags = 0;
  float xPos = 0;
  float yPos = 0;
  void DrawButtons();
};
