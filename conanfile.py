from conans import ConanFile


class CMakeTemplateConan(ConanFile):
  name = "nes-emulator"
  version = "1.0"
  settings = "os", "compiler", "build_type", "arch"
  generators = "cmake"

  def requirements(self):
    self.requires("gtest/1.8.1", private=True, override=False)
    self.requires("imgui/1.88", private=True, override=False)
    self.requires("glfw/3.3.8", private=True, override=False)
    self.requires("glew/2.2.0", private=True, override=False)
    self.requires("fmt/9.1.0", private=True, override=False)
    self.requires("glm/0.9.9.8", private=True, override=False)
    self.requires("lodepng/cci.20200615", private=True, override=False)
    self.requires("miniaudio/0.11.11", private=True, override=False)

  def build_requirements(self):
    pass

  def imports(self):
    source = self.deps_cpp_info["imgui"].resdirs[0] + "/bindings/"
    destination = "bindings"

    self.copy("imgui_impl_glfw.h", dst=destination, src=source)
    self.copy("imgui_impl_opengl3.h", dst=destination, src=source)
    self.copy("imgui_impl_opengl3_loader.h", dst=destination, src=source)
    self.copy("imgui_impl_glfw.cpp", dst=destination, src=source)
    self.copy("imgui_impl_opengl3.cpp", dst=destination, src=source)
