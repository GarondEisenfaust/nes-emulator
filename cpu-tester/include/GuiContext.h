#pragma once
#include "GL/glew.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "windows/GuiWindow.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

class GuiContext {
 public:
  GuiContext();
  ~GuiContext();
  void Loop();
  std::vector<GuiWindow*> mClientWindows;

 private:
  GLFWwindow* mWindow;
  ImVec4 mClearColor;
};
