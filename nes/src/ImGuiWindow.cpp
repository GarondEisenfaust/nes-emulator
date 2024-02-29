#include "ImGuiWindow.h"
#include "imgui.h"

ImGuiWindow::ImGuiWindow(const std::string& windowName) : mWindowName(windowName) {}

void ImGuiWindow::Draw() {
  ImGui::Begin(mWindowName.c_str(), &mShow);
  if (mDrawingFunction) {
    mDrawingFunction();
  }
  ImGui::End();
}
