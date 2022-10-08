#include "GuiContext.h"
#include <iostream>

GuiContext::GuiContext() : mClearColor(0.45f, 0.55f, 0.60f, 1.00f) {
  if (!glfwInit()) {
    throw std::runtime_error("ERROR: could not start GLFW3!\n");
  }

  mWindow = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);

  if (!mWindow) {
    throw std::runtime_error("Could not open window with GLFW3!");
  }

  glfwMakeContextCurrent(mWindow);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
  ImGui_ImplOpenGL3_Init();
}

void GuiContext::Loop() {
  while (!glfwWindowShouldClose(mWindow)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for (auto& clientWindow : mClientWindows) {
      clientWindow->Render();
    }

    // Rendering
    ImGui::Render();
    int displayW, displayH;
    glfwGetFramebufferSize(mWindow, &displayW, &displayH);
    glViewport(0, 0, displayW, displayH);
    glClearColor(mClearColor.x * mClearColor.w, mClearColor.y * mClearColor.w, mClearColor.z * mClearColor.w,
                 mClearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(mWindow);
  }
}

GuiContext::~GuiContext() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(mWindow);
  glfwTerminate();
}