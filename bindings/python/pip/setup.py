"""Build of the tfel-mfront pip package.

TFEL is configured, built and installed with cmake. The installation is
then laid out as follows:

- the python modules (tfel, mfront, mtest, mfm_test_generator) are placed
  at the root of the package,
- the executables, libraries, headers and data are placed in the bin,
  lib, include and share directories of the tfel module, which is thus
  the TFELHOME directory.
"""

import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

from setuptools import setup
from setuptools.command.build_ext import build_ext
from setuptools.dist import Distribution

pip_directory = Path(__file__).resolve().parent
tfel_sources = pip_directory.parents[2]


def get_tfel_version():
    """return the TFEL version, as defined in the main CMakeLists.txt"""
    cmakelists = (tfel_sources / "CMakeLists.txt").read_text()
    version = re.search(r'tfel_project\(\s*(\d+)\s+(\d+)\s+"?(\d+)"?\s*\)', cmakelists)
    if version is None:
        raise RuntimeError("TFEL version not found in CMakeLists.txt")
    version = ".".join(version.groups())
    if re.search(r"set\(TFEL_DEVELOPMENT_VERSION\s+ON\)", cmakelists):
        return version + "-dev"
    return version


tfel_version = get_tfel_version()
# environment variable defining the installation directory of this version
# of TFEL. It takes precedence over TFELHOME (see src/Config/GetInstallPath.cxx)
tfelhome_variable = "TFELHOME_" + tfel_version.replace(".", "_").replace("-", "_")

# python code which sets the TFELHOME variable, inserted at the beginning of
# the __init__.py files of the python modules. The argument gives the path to
# the tfel module relatively to the module.
tfelhome_setup = """\
# added by the tfel-mfront pip package
import os as _os
_os.environ["{variable}"] = _os.path.normpath(
    _os.path.join(_os.path.dirname(_os.path.abspath(__file__)), "{path}"))
"""


class BinaryDistribution(Distribution):
    # compiled modules inside: platform-specific wheel, not py3-none-any
    def has_ext_modules(self):
        return True


def build_and_install_tfel(build_directory, install_directory):
    import pybind11

    cmake_options = [
        "-DCMAKE_BUILD_TYPE=Release",
        "-DCMAKE_INSTALL_PREFIX=" + str(install_directory),
        "-DTFEL_PYTHON_SITE_PACKAGES_DIR=site-packages",
        "-DPython_EXECUTABLE=" + sys.executable,
        "-Dpybind11_DIR=" + pybind11.get_cmake_dir(),
        "-Dunversioned-libraries=ON",
        "-Denable-python-bindings=ON",
        "-Denable-numpy-support=ON",
        "-Denable-portable-build=ON",
        "-Denable-reference-doc=OFF",
        "-Denable-testing=OFF",
        "-Denable-aster=ON",
        "-Denable-abaqus=ON",
        "-Denable-ansys=ON",
        "-Denable-zmat=OFF",
        "-Denable-cyrano=ON",
        "-Denable-calculix=ON",
        "-Denable-comsol=OFF",
        "-Denable-diana-fea=ON",
        "-Denable-lsdyna=OFF",
    ]
    if os.name == "nt":
        cmake_options += ["-A", "x64"]
    jobs = os.environ.get("CMAKE_BUILD_PARALLEL_LEVEL", str(os.cpu_count() or 1))
    subprocess.run(["cmake", "-S", str(tfel_sources), "-B", str(build_directory)]
                   + cmake_options, check=True)
    subprocess.run(["cmake", "--build", str(build_directory), "--config",
                    "Release", "--parallel", jobs], check=True)
    subprocess.run(["cmake", "--install", str(build_directory), "--config",
                    "Release"], check=True)


def insert_tfelhome_setup(init_file, path_to_tfel_module):
    """insert the setup of the TFELHOME variable in a __init__.py file,
    after the `from __future__` statements which must come first"""
    lines = init_file.read_text().splitlines(keepends=True)
    position = 0
    while position < len(lines) and lines[position].startswith("from __future__"):
        position += 1
    setup_code = tfelhome_setup.format(variable=tfelhome_variable,
                                       path=path_to_tfel_module)
    lines.insert(position, setup_code)
    init_file.write_text("".join(lines))


def is_binary(path, magic_numbers):
    with open(path, "rb") as f:
        return f.read(4) in magic_numbers


def set_relative_runtime_paths(package_directory):
    """make all the binaries look for the libraries in tfel/lib.

    The installed binaries either have no runtime path (linux) or an
    absolute one (macOS). auditwheel and delocate keep the libraries
    already in the wheel when the runtime paths point to them.

    On linux, DT_RPATH is used rather than DT_RUNPATH, since it is
    searched before LD_LIBRARY_PATH: the binaries of the package use the
    libraries of the package, even if LD_LIBRARY_PATH points to another
    TFEL installation, whose libraries may have the same names."""
    library_directory = package_directory / "tfel" / "lib"
    if sys.platform.startswith("linux"):
        magic_numbers = (b"\x7fELF",)
    elif sys.platform == "darwin":
        magic_numbers = (b"\xcf\xfa\xed\xfe", b"\xca\xfe\xba\xbe")
    else:
        return
    for path in sorted(package_directory.rglob("*")):
        if path.is_symlink() or not path.is_file():
            continue
        if not is_binary(path, magic_numbers):
            continue
        relative_path = os.path.relpath(library_directory, path.parent)
        if sys.platform.startswith("linux"):
            rpath = "$ORIGIN" if relative_path == "." else "$ORIGIN/" + relative_path
            subprocess.run(["patchelf", "--force-rpath", "--set-rpath", rpath,
                            str(path)], check=True)
        else:
            load_commands = subprocess.run(["otool", "-l", str(path)], check=True,
                                           capture_output=True,
                                           text=True).stdout.splitlines()
            rpaths = [load_commands[i + 2].split()[1]
                      for i, line in enumerate(load_commands)
                      if line.strip() == "cmd LC_RPATH"]
            options = []
            for rpath in rpaths:
                options += ["-delete_rpath", rpath]
            rpath = "@loader_path" if relative_path == "." else "@loader_path/" + relative_path
            options += ["-add_rpath", rpath]
            subprocess.run(["install_name_tool"] + options + [str(path)], check=True)
            # install_name_tool invalidates the signature of the binary
            subprocess.run(["codesign", "--force", "--sign", "-", str(path)],
                           check=True)


def layout_package(install_directory, package_directory):
    """lay out the TFEL installation as a python package"""
    shutil.copytree(install_directory / "site-packages", package_directory,
                    dirs_exist_ok=True)
    tfel_module = package_directory / "tfel"
    for directory in ("bin", "lib", "include", "share"):
        if (install_directory / directory).is_dir():
            shutil.copytree(install_directory / directory, tfel_module / directory,
                            dirs_exist_ok=True)
    insert_tfelhome_setup(tfel_module / "__init__.py", ".")
    for module in ("mtest", "mfm_test_generator"):
        insert_tfelhome_setup(package_directory / module / "__init__.py",
                              "../tfel")
    launcher = (pip_directory / "tfel_mfront_launcher.py.in").read_text()
    (package_directory / "tfel_mfront_launcher.py").write_text(
        launcher.replace("@TFELHOME_VARIABLE@", tfelhome_variable))
    set_relative_runtime_paths(package_directory)


class build_tfel(build_ext):
    """build TFEL with cmake and fill the build directory of the package.

    build_ext is used since it is the only build command run for a
    distribution without python packages declared to setuptools"""

    def run(self):
        build_temp = Path(self.build_temp).resolve()
        install_directory = build_temp / "install"
        build_and_install_tfel(build_temp / "build", install_directory)
        layout_package(install_directory, Path(self.build_lib))


setup(
    # 5.2.0-dev is not a valid PEP 440 version, 5.2.0.dev0 is
    version=tfel_version.replace("-dev", ".dev0"),
    distclass=BinaryDistribution,
    cmdclass={"build_ext": build_tfel},
)
