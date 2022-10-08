#include "windows/MemoryWindow.h"
#include "imgui.h"
#include <iostream>

MemoryWindow::MemoryWindow(const char* title, const MemoryReader& memoryDisplay)
    : GuiWindow(title), mMemoryReader(memoryDisplay) {}

void MemoryWindow::Content() {
  const auto toDisplay = mMemoryReader.CreateDisplayStream()->str();
  ImGui::Text(toDisplay.c_str());
}
