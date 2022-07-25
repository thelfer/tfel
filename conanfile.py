import os
from conans import ConanFile, CMake


class TFELConan(ConanFile):
    name = "TFEL"
    author = "Thomas Helfer (tfel-contact@cea.fr)"
    version = "3.1-dev"
    license = "GPL/CECILL-A"
    url = "http://tfel.sourceforge.net"
    description = """TFEL is a collaborative development of CEA and EDF.
    MFront is a code generator which translates a set of closely related 
    domain specific languages into plain C++ on top of the  TFEL library.
    Those languages cover three kinds of material knowledge:
    - material properties (for instance the Young modulus, 
      the thermal conductivity, etc.)
    - mechanical behaviours. Numerical performances of generated
      mechanical behaviours were given a particular attention.
      Various benchmarks show that MFront implementations are 
      competitive with native implementations available in the Cast3M,
      Code-Aster, Abaqus Standard and Cyrano3 solvers.
    - simple point-wise models, such as material swelling used in 
      fuel performance codes."""
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    requires = "Boost.Python/1.65.1@bincrafters/stable"
    default_options = "shared=True"
    generators = "cmake"

    def source(self):
        self.run("git clone https://github.com/thelfer/tfel.git")

    def build(self):
        cmake = CMake(self, parallel=True)
        args = [
            '-DCMAKE_INSTALL_PREFIX="%s"' % self.package_folder,
            '-Denable-python-bindings=ON', '-Dlocal-castem-header=ON',
            '-Denable-aster=ON', '-Denable-europlexus=ON',
            '-Denable-abaqus=ON', '-Denable-calculix=ON',
            '-DTFEL_SVN_REVISION=0'
        ]
        self.run(
            'cmake %s/tfel %s %s' %
            (self.conanfile_directory, cmake.command_line, ' '.join(args)))
        self.run("cmake --build . --target install %s" % cmake.build_config)

    def package(self):
        pass

    def package_info(self):
        self.cpp_info.libs = ["tfel"]
        self.env_info.PATH.append(os.path.join(self.package_folder, "bin"))
        if not self.settings.os == "Windows":
            self.env_info.LD_LIBRARY_PATH.append(
                os.path.join(self.package_folder, "lib"))
            self.env_info.PYTHONPATH.append(
                os.path.join(self.package_folder,
                             "lib/python2.7/site-packages/"))
