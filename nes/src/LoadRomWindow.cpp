#include "LoadRomWindow.h"
#include "imgui.h"
#include <filesystem>
#include <iostream>

LoadRomWindow::LoadRomWindow(const std::string& windowName, const std::string& romDirectory)
    : ImGuiWindow(windowName.c_str()), mRomDirectory(romDirectory) {
  namespace fs = std::filesystem;
  mShow = false;
  mRomDirectoryInitialized = !mRomDirectory.empty();
  if (mRomDirectoryInitialized) {
    fs ::directory_iterator romDirectoryEntries(mRomDirectory);
    std::transform(fs::begin(romDirectoryEntries), fs::end(romDirectoryEntries), std::back_inserter(mRomPaths),
                   [](auto romName) { return romName.path(); });
    mShow = !mRomPaths.empty();
    mDrawingFunction = [&]() {
      for (auto romPath : mRomPaths) {
        if (ImGui::Button(romPath.c_str())) {
          mCurrentRomPath = romPath;
          break;
        }
      }
    };
  }
}

void LoadRomWindow::Draw() {
  if (mRomDirectoryInitialized) {
    ImGuiWindow::Draw();
  }
}
