from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeToolchain, CMakeDeps, CMake
from conan.tools.files import copy

class MfsmConan(ConanFile):
  name = "mfsm"
  version = "1.2"
  license = "BSL-1.0"
  author = "Philippe Serreault (philippe.serreault@gmail.com)"
  url = "https://github.com/SBAM/mfsm"
  description = "Modern c++ header-only meta state-machine library"
  topics = ("c++", "state-machine", "header-only")
  settings = "os", "arch", "compiler", "build_type"
  exports_sources = "CMakeLists.txt", "include/*", "unit-tests/*"
  no_copy_source = True
  generators = "CMakeToolchain", "CMakeDeps"

  def build_requirements(self):
    self.tool_requires("cmake/3.25.3")
    self.test_requires("boost/1.81.0")

  def layout(self):
    cmake_layout(self)

  def build(self):
    self.cmake = CMake(self)
    self.cmake.configure(cli_args=["--no-warn-unused-cli"])
    if not self.conf.get("tools.build:skip_test", default=False):
      self.cmake.build()

  def test(self):
    self.cmake.test()

  def package(self):
    self.cmake.install()

  def package_info(self):
    self.cpp_info.bindirs = []
    self.cpp_info.libdirs = []

  def package_id(self):
    self.info.clear()
