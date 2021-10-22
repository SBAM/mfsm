from conans import ConanFile

class MfsmConan(ConanFile):
  name = "mfsm"
  version = "1.0"
  license = "BSL-1.0"
  author = "Philippe Serreault (philippe.serreault@gmail.com)"
  url = "https://github.com/SBAM/mfsm"
  description = "Modern c++ header-only meta state-machine library"
  topics = ("c++", "state-machine", "header-only")
  exports_sources = "include/*"
  no_copy_source = True

  def package(self):
    self.copy("*.hpp")
    self.copy("*.ipp")

  def package_id(self):
    self.info.header_only()
