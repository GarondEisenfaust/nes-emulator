#pragma once
#include "Cpu.h"
#include "GuiWindow.h"
#include <cstdint>
#include <map>
#include <string>

class DisassamblerWindow : public GuiWindow {
 public:
  DisassamblerWindow(Cpu& cpu);

 protected:
  virtual void Content() override;

 private:
  Cpu& mCpu;
  std::map<uint16_t, std::string> mDissasabledCode;
};