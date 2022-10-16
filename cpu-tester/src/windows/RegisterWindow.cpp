#include "windows/RegisterWindow.h"
#include "fmt/core.h"
#include "imgui.h"
#include <string>

RegisterWindow::RegisterWindow(const char* title, Processor6502& processor6502)
    : GuiWindow(title), mProcessor6502(processor6502) {}

void RegisterWindow::Content() {
  mProcessor6502.status = 8;
  ImGui::Text(fmt::format("status:           NVUBDIZC", mProcessor6502.status).c_str());
  ImGui::Text(fmt::format("                  {:0>8b}", mProcessor6502.status).c_str());
  ImGui::Text("");
  ImGui::Text(fmt::format("program counter:  0x{:0>2x}", mProcessor6502.pc).c_str());
  ImGui::Text(fmt::format("stack pointer:    0x{:0>2x}", mProcessor6502.stackPointer).c_str());
  ImGui::Text(fmt::format("accumulator:      0x{:0>2x}", mProcessor6502.a).c_str());
  ImGui::Text(fmt::format("x:                0x{:0>2x}", mProcessor6502.x).c_str());
  ImGui::Text(fmt::format("y:                0x{:0>2x}", mProcessor6502.y).c_str());
}
