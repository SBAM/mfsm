from conan import ConanFile
from conan.tools.cmake import cmake_layout, CMakeToolchain, CMakeDeps, CMake
from conan.tools.files import copy

class MfsmConan(ConanFile):
  name = "mfsm"
  version = "1.3"
  license = "BSL-1.0"
  author = "Philippe Serreault (philippe.serreault@gmail.com)"
  url = "https://github.com/SBAM/mfsm"
  description = "Modern c++ header-only meta state-machine library"
  topics = ("c++", "state-machine", "header-only")
  settings = "os", "arch", "compiler", "build_type"
  exports_sources = "CMakeLists.txt", "include/*", "unit-tests/*"
  no_copy_source = True
  generators = "CMakeToolchain", "CMakeDeps"
  default_options = { "boost/*:without_atomic": True,
                      "boost/*:without_charconv": True,
                      "boost/*:without_chrono": True,
                      "boost/*:without_cobalt": True,
                      "boost/*:without_container": True,
                      "boost/*:without_context": True,
                      "boost/*:without_contract": True,
                      "boost/*:without_coroutine": True,
                      "boost/*:without_date_time": True,
                      "boost/*:without_exception": False,
                      "boost/*:without_fiber": True,
                      "boost/*:without_filesystem": True,
                      "boost/*:without_graph": True,
                      "boost/*:without_graph_parallel": True,
                      "boost/*:without_iostreams": True,
                      "boost/*:without_json": True,
                      "boost/*:without_locale": True,
                      "boost/*:without_log": True,
                      "boost/*:without_math": True,
                      "boost/*:without_mpi": True,
                      "boost/*:without_nowide": True,
                      "boost/*:without_process": True,
                      "boost/*:without_program_options": True,
                      "boost/*:without_python": True,
                      "boost/*:without_random": True,
                      "boost/*:without_regex": True,
                      "boost/*:without_serialization": True,
                      "boost/*:without_stacktrace": True,
                      "boost/*:without_system": True,
                      "boost/*:without_test": False,
                      "boost/*:without_thread": True,
                      "boost/*:without_timer": True,
                      "boost/*:without_type_erasure": True,
                      "boost/*:without_url": True,
                      "boost/*:without_wave": True }

  def build_requirements(self):
    self.test_requires("boost/[>=1.86.0]")

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
