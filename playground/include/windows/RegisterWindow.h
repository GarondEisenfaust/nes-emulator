#pragma once
#include "GuiWindow.h"
#include "Processor6502.h"

class RegisterWindow : public GuiWindow {
 public:
  RegisterWindow(Processor6502& processor6502);
  ~RegisterWindow() = default;
  void Content() override;

 private:
  Processor6502& mProcessor6502;

  uint8_t mOldFetched = 0x00;
  uint8_t mOldA = 0x00;
  uint8_t mOldX = 0x00;
  uint8_t mOldY = 0x00;
  uint8_t mOldStackPointer = 0x00;
  uint16_t mOldPc = 0x0000;
  uint8_t mOldStatus = 0x00;
  uint8_t mOldCycles = 0;
  uint16_t mOldAddrAbs = 0x0000;
  uint16_t mOldAddrRel = 0x00;
  uint8_t mOldOpcode = 0x00;
  uint16_t mOldTemp = 0x0000;

  uint8_t mOldNestest02 = 0x00;
  uint8_t mOldNestest03 = 0x00;
};
