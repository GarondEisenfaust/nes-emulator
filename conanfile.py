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
    self.requires("glm/1.0.1", headers=True)
    self.requires("miniaudio/0.11.11", headers=True)
    self.tool_requires("android-ndk/r27")

    if self.settings.os != "Android":
      self.requires("gtest/1.8.1", headers=True)
      self.requires("imgui/1.91.0", headers=True)
      self.requires("glfw/3.4", headers=True)
      self.requires("glad/0.1.36", headers=True)

  def build_requirements(self):
    if self.settings.os != "Android":
      pass

  def package_info(self):
    self.conf_info.define("tools.android:ndk_path", os.path.join(self.package_folder, "ndk"))

  def generate(self):
    if self.settings.os != "Android":
      source = os.path.join(self.dependencies["imgui"].package_folder, "res", "bindings")
      target = os.path.join(self.build_folder, "bindings")

      copy(self, "*glfw*", source, target)
      copy(self, "*opengl3*", source, target)

  def layout(self):
    build_dir_name = "{}-{}-{}".format(self.settings.os, self.settings.arch, self.settings.build_type).lower()
    self.folders.build = os.path.join("build", build_dir_name)
    self.folders.build_folder_vars = ['settings.os', 'settings.arch', 'settings.build_type']
    self.folders.generators = os.path.join(self.folders.build, "generators")
