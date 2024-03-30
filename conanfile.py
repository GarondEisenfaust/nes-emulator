from conans import ConanFile


class CMakeTemplateConan(ConanFile):
  name = "nes-emulator"
  version = "1.0"
  settings = "os", "compiler", "build_type", "arch"
  generators = "cmake"

  def requirements(self):
    self.requires("gtest/1.8.1", private=True, override=False)
    self.requires("imgui/1.87", private=True, override=False)
    self.requires("glfw/3.4", private=True, override=False)
    self.requires("glad/0.1.36", private=True, override=False) 
    self.requires("miniaudio/0.11.11", private=True, override=False)

  def build_requirements(self):
    pass

  def imports(self):
    source = "./res/bindings/"
    destination = "bindings"

    self.copy("imgui_impl_glfw.h", dst=destination, src=source)
    self.copy("imgui_impl_opengl3.h", dst=destination, src=source)
    self.copy("imgui_impl_opengl3_loader.h", dst=destination, src=source)
    self.copy("imgui_impl_glfw.cpp", dst=destination, src=source)
    self.copy("imgui_impl_opengl3.cpp", dst=destination, src=source)
