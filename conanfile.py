from conans import ConanFile, CMake

class IOMgrConan(ConanFile):
    name = "iomgr"
    version = "0.1.0"
    license = "Proprietary"
    description = "iomgr"

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}

    build_requires = (("sds_logging/[>=0.1.2,<1.0]@demo/dev"))

    generators = "cmake"
    default_options = "shared=False"
    exports_sources = "*"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include/iomgr", src="src", keep_path=False)
        self.copy("*.hpp", dst="include/iomgr", src="src", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)

    def package_info(self):
        if self.options.shared:
            self.cpp_info.libs = ['libiomgr.so']
        else:
            self.cpp_info.libs = ['libiomgr.a']
