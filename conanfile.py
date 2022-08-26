from conans import ConanFile


class CMakeTemplateConan(ConanFile):
  name = "cmake-template"
  version = "1.0"
  settings = "os", "compiler", "build_type", "arch"
  generators = "cmake_find_package"

  def requirements(self):
    self.requires("gtest/cci.20210126", private=True, override=False)

  def build_requirements(self):
    pass

  def imports(self):
    pass
