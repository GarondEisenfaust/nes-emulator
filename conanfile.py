import os

from conan import ConanFile
from conan.tools.cmake import cmake_layout
from conan.tools.files import copy


class NesEmulator(ConanFile):
  name = "nes-emulator"
  version = "1.0"
  settings = "os", "compiler", "build_type", "arch"
  generators = "CMakeToolchain", "CMakeDeps"

  def requirements(self):
    self.requires("gtest/1.8.1", headers=True)
    self.requires("imgui/1.91.0", headers=True)
    self.requires("glfw/3.4", headers=True)
    self.requires("glad/0.1.36", headers=True)
    self.requires("miniaudio/0.11.11", headers=True)

  def generate(self):
    source = os.path.join(self.dependencies["imgui"].package_folder, "res",
                          "bindings")
    target = os.path.join(self.build_folder, "bindings")

    copy(self, "*glfw*", source, target)
    copy(self, "*opengl3*", source, target)

  # def layout(self):
  #   cmake_layout(self)
