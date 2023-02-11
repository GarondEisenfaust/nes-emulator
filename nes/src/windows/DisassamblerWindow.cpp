#include "windows/DisassamblerWindow.h"
#include "imgui.h"
#include <array>
#include <iostream>
#include <string>

DisassamblerWindow::DisassamblerWindow(Processor6502* cpu) : GuiWindow("Disassambler", true), mCpu(cpu) {
  mDissasabledCode = cpu->Disassemble(0x0000, 0xFFFF);
}

void DisassamblerWindow::Content() {
  auto iterator = mDissasabledCode.find(mCpu->pc);
  auto current = iterator->second;
  std::array<std::string, 10> previousTen;
  for (auto i = 0; i < 10; i++) {
    iterator--;
    previousTen[9 - i] = iterator->second;
  }

  iterator = mDissasabledCode.find(mCpu->pc);
  std::array<std::string, 10> nextTen;
  for (auto i = 0; i < 10; i++) {
    iterator++;
    if (iterator->first >= 0xFFFF) {
      break;
    }
    nextTen[i] = iterator->second;
  }

  for (auto line : previousTen) {
    ImGui::Text(line.c_str());
  }
  ImGui::TextColored(ImVec4(1, 0, 0, 1), current.c_str());
  for (auto line : nextTen) {
    ImGui::Text(line.c_str());
  }
}
