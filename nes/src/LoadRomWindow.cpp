#include "LoadRomWindow.h"
#include "Bus.h"
#include "imgui.h"
#include <algorithm>
#include <filesystem>
#include <iostream>

LoadRomWindow::LoadRomWindow(const std::string& windowName, const std::string& romDirectory, Bus* bus)
    : mWindowName(windowName), mRomDirectory(romDirectory), mBus(bus), mStartCollapsed(bus->CartridgeInserted()) {
  mShow = !mRomDirectory.empty();
  if (!mShow) {
    return;
  }

  namespace fs = std::filesystem;
  fs ::directory_iterator romDirectoryEntries(mRomDirectory);
  std::transform(fs::begin(romDirectoryEntries), fs::end(romDirectoryEntries), std::back_inserter(mRomPaths),
                 [](auto romName) { return romName.path(); });
  mShow = !mRomPaths.empty();
}

void LoadRomWindow::SetBus(Bus* bus) { mBus = bus; }

void LoadRomWindow::DrawButtons() {
  for (auto romPath : mRomPaths) {
    if (ImGui::Button(romPath.c_str())) {
      mBus->InsertCartridge(std::make_shared<Cartridge>(romPath));
      mBus->Reset();
      break;
    }
  }
}

void LoadRomWindow::Draw() {
  if (!mShow) {
    return;
  }
  ImGui::SetNextWindowPos({xPos, yPos});
  ImGui::SetNextWindowSizeConstraints({400, 600}, {400, 600});
  ImGui::SetNextWindowCollapsed(mStartCollapsed, ImGuiCond_Appearing);
  if (ImGui::Begin(mWindowName.c_str())) {
    DrawButtons();
  }
  ImGui::End();
}
