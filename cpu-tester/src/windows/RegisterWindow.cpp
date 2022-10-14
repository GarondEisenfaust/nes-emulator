#include "windows/RegisterWindow.h"
#include "fmt/core.h"
#include "imgui.h"
#include <string>

RegisterWindow::RegisterWindow(const char* title, Processor6502& processor6502)
    : GuiWindow(title), mProcessor6502(processor6502) {}

void RegisterWindow::Content() {
  ImGui::Text(fmt::format("accumulator:       {}", mProcessor6502.a).c_str());
  ImGui::Text(fmt::format("x:                 {}", mProcessor6502.x).c_str());
  ImGui::Text(fmt::format("y:                 {}", mProcessor6502.y).c_str());
  ImGui::Text(fmt::format("status:            {}", mProcessor6502.status).c_str());
  ImGui::Text(fmt::format("program counter:   {}", mProcessor6502.pc).c_str());
  ImGui::Text(fmt::format("stack pointer:     {}", mProcessor6502.stackPointer).c_str());
}
