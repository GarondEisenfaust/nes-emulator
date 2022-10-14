#include "windows/RegisterWindow.h"
#include "imgui.h"
#include <string>

RegisterWindow::RegisterWindow(const char* title, Processor6502& processor6502)
    : GuiWindow(title), mProcessor6502(processor6502) {}

void RegisterWindow::Content() { ImGui::Text(("accumulator: " + std::to_string(mProcessor6502.a)).c_str()); }
