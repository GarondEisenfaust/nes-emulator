#include "windows/GuiWindow.h"
#include "imgui.h"

GuiWindow::GuiWindow(const char* title, bool open) : mTitle(title), mOpen(open) {}

void GuiWindow::Render() {
  ImGui::Begin(mTitle, &mOpen);
  Content();
  ImGui::End();
}

void GuiWindow::Content() {}
