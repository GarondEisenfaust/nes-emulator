#pragma once
#include "GuiWindow.h"
#include "Processor6502.h"

class RegisterWindow : public GuiWindow {
 public:
  RegisterWindow(const char* title, Processor6502& processor6502);
  ~RegisterWindow() = default;
  void Content() override;

 private:
  Processor6502& mProcessor6502;
};
