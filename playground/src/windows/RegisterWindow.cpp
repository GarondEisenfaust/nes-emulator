#include "windows/RegisterWindow.h"
#include "Bus.h"
#include "fmt/core.h"
#include "imgui.h"
#include <string>

RegisterWindow::RegisterWindow(Processor6502& processor6502) : GuiWindow("Registers"), mProcessor6502(processor6502) {}

template <size_t N>
struct StringLiteral {
  constexpr StringLiteral(const char (&str)[N]) { std::copy_n(str, N, value); }

  char value[N];
};

template <class TYPE, StringLiteral FORMAT_STRING>
void CompareUpdateOldAndDisplay(TYPE& oldValue, TYPE newValue) {
  auto color = ImVec4(1, 1, 1, 1);
  if (oldValue != newValue) {
    color = ImVec4(1, 0, 0, 1);
  }

  ImGui::TextColored(color, fmt::format(FORMAT_STRING.value, newValue).c_str());
  oldValue = newValue;
}

void RegisterWindow::Content() {
  ImGui::Text("status:           NVUBDIZC");
  CompareUpdateOldAndDisplay<uint8_t, "                  {:0>8b}">(mOldStatus, mProcessor6502.status);
  ImGui::Text("");

  CompareUpdateOldAndDisplay<uint16_t, "program counter:  {:#06x}">(mOldPc, mProcessor6502.pc);
  CompareUpdateOldAndDisplay<uint8_t, "stack pointer:    {:#04x}">(mOldStackPointer, mProcessor6502.stackPointer);
  CompareUpdateOldAndDisplay<uint8_t, "opcode:           {:#04x}">(mOldOpcode, mProcessor6502.opcode);
  CompareUpdateOldAndDisplay<uint16_t, "addrAbs:          {:#06x}">(mOldAddrAbs, mProcessor6502.addrAbs);
  CompareUpdateOldAndDisplay<uint16_t, "addrRel:          {:#06x}">(mOldAddrRel, mProcessor6502.addrRel);
  CompareUpdateOldAndDisplay<uint8_t, "cycles:           {:#04x}">(mOldCycles, mProcessor6502.cycles);
  CompareUpdateOldAndDisplay<uint8_t, "fetched:          {:#04x}">(mOldFetched, mProcessor6502.fetched);

  ImGui::Text("");
  CompareUpdateOldAndDisplay<uint8_t, "accumulator:      {:#04x}">(mOldA, mProcessor6502.a);
  CompareUpdateOldAndDisplay<uint8_t, "x:                {:#04x}">(mOldX, mProcessor6502.x);
  CompareUpdateOldAndDisplay<uint8_t, "y:                {:#04x}">(mOldY, mProcessor6502.y);
  ImGui::Text("");
  CompareUpdateOldAndDisplay<uint16_t, "temp:             {:#06x}">(mOldTemp, mProcessor6502.temp);
  ImGui::Text("");
  ImGui::Text("nes test memory locations:");
  CompareUpdateOldAndDisplay<uint8_t, "0x02:             {:#04x}">(mOldNestest02,
                                                                   mProcessor6502.mBus->CpuRead(0x0002, true));
  CompareUpdateOldAndDisplay<uint8_t, "0x03:             {:#04x}">(mOldNestest03,
                                                                   mProcessor6502.mBus->CpuRead(0x0003, true));
}

uint8_t opcode = 0x00;  // Is the instruction byte
uint16_t temp = 0x0000;