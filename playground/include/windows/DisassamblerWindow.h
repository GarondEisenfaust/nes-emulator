#pragma once
#include "GuiWindow.h"
#include "Processor6502.h"
#include <cstdint>
#include <map>
#include <string>

class DisassamblerWindow : public GuiWindow {
 public:
  DisassamblerWindow(Processor6502* cpu);

 protected:
  virtual void Content() override;

 private:
  Processor6502* mCpu;
  std::map<uint16_t, std::string> mDissasabledCode;
};