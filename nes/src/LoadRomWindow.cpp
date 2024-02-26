#include "LoadRomWindow.h"
#include "imgui.h"
#include <filesystem>
#include <iostream>

LoadRomWindow::LoadRomWindow(const char* windowName, const char* romDirectory)
    : ImGuiWindow(windowName), mRomDirectory(romDirectory) {
  namespace fs = std::filesystem;

  fs ::directory_iterator romDirectoryEntries(mRomDirectory);
  std::transform(fs::begin(romDirectoryEntries), fs::end(romDirectoryEntries), std::back_inserter(mRomPaths),
                 [](auto romName) { return romName.path(); });

  mDrawingFunction = [&]() {
    for (auto romPath : mRomPaths) {
      if (ImGui::Button(romPath.c_str())) {
        mCurrentRomPath = romPath;
        break;
      }
    }
  };
}
