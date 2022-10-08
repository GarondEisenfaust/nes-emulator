from conans import ConanFile


class CMakeTemplateConan(ConanFile):
  name = "nes-emulator"
  version = "1.0"
  settings = "os", "compiler", "build_type", "arch"
  generators = "cmake"

  def requirements(self):
    self.requires("gtest/cci.20210126", private=True, override=False)

  def build_requirements(self):
    pass

  def imports(self):
    pass
