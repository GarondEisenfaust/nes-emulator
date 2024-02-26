#include "ImGuiWindow.h"
#include "imgui.h"

ImGuiWindow::ImGuiWindow(const char* windowName) : mWindowName(windowName) {}

void ImGuiWindow::Draw() {
  ImGui::Begin(mWindowName, &mShow);
  mDrawingFunction();
  ImGui::End();
}
