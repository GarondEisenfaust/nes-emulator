#include "windows/RegisterWindow.h"
#include "Bus.h"
#include "fmt/core.h"
#include "imgui.h"
#include <string>

RegisterWindow::RegisterWindow(Cpu& Cpu) : GuiWindow("Registers"), mCpu(Cpu) {}

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
  CompareUpdateOldAndDisplay<uint8_t, "                  {:0>8b}">(mOldStatus, mCpu.status.reg);
  ImGui::Text("");

  CompareUpdateOldAndDisplay<uint16_t, "program counter:  {:#06x}">(mOldPc, mCpu.pc);
  CompareUpdateOldAndDisplay<uint8_t, "stack pointer:    {:#04x}">(mOldStackPointer, mCpu.stackPointer);
  CompareUpdateOldAndDisplay<uint8_t, "opcode:           {:#04x}">(mOldOpcode, mCpu.opcode);
  CompareUpdateOldAndDisplay<uint16_t, "addrAbs:          {:#06x}">(mOldAddrAbs, mCpu.addrAbs);
  CompareUpdateOldAndDisplay<uint16_t, "addrRel:          {:#06x}">(mOldAddrRel, mCpu.addrRel);
  CompareUpdateOldAndDisplay<uint8_t, "cycles:           {}">(mOldCycles, mCpu.cycles);
  CompareUpdateOldAndDisplay<uint8_t, "fetched:          {:#04x}">(mOldFetched, mCpu.fetched);

  ImGui::Text("");
  CompareUpdateOldAndDisplay<uint8_t, "accumulator:      {0:#04x} {0}">(mOldA, mCpu.a);
  CompareUpdateOldAndDisplay<uint8_t, "x:                {0:#04x} {0}">(mOldX, mCpu.x);
  CompareUpdateOldAndDisplay<uint8_t, "y:                {0:#04x} {0}">(mOldY, mCpu.y);
  // ImGui::Text("");
  // CompareUpdateOldAndDisplay<uint16_t, "temp:             {0:#06x} {0}">(mOldTemp, mCpu.temp);
  ImGui::Text("");
  ImGui::Text("nes test memory locations:");
  CompareUpdateOldAndDisplay<uint8_t, "0x02:             {:#04x}">(mOldNestest02, mCpu.mBus->CpuRead(0x0002, true));
  CompareUpdateOldAndDisplay<uint8_t, "0x03:             {:#04x}">(mOldNestest03, mCpu.mBus->CpuRead(0x0003, true));
}

uint8_t opcode = 0x00;  // Is the instruction byte
uint16_t temp = 0x0000;