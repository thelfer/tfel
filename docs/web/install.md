---
title: Installing `TFEL/MFront`
author: Thomas Helfer
date: 29/20/2021
lang: en-EN
numbersections: true
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

The latest version of this document is always available at
<https://thelfer.github.io/tfel/web/install.html>.

> **Note** `TFEL` can be installed along with 
> [`Cast3M`](http://www-cast3m.cea.fr/) (>2019). *No extra
> installation is required*.

> **Note** `TFEL` is a part of the
> [`code-aster`](http://www.code-aster.org/) (>12.03) and
> [`Salome-Meca`](http://www.code-aster.org/) (> 2015.1) packages: no extra
> installation is required if you use one of these products*

> **Note** `TFEL` is available as a `spack` package.
>
> One easy way to install `TFEL` under `LiNuX` and `MacOs`
> is to use the following instructions:
>
> ~~~~{.bash}
> $ git clone --single-branch -b develop https://github.com/spack/spack.git 
> $ . spack/share/spack/setup-env.sh
> $ spack install tfel@master
> ~~~~
>
> The `TFEL` package can then be loaded as follows:
>
> ~~~~{.bash}
> $ spack load tfel@master
> ~~~~

It refers to the **current development sources**, instructions for
specific released versions are included with the sources and detailed
in the `INSTALL` and the `INSTALL-cmake` files located at the root
directory of the sources.

This document describes the generic installation procedure for `TFEL`
from the sources on posix-compliant systems. Please note that the main
systems on which `TFEL` was developed is `Linux`. Extensive testing on
other posix-compliant operating systems, notably `FreeBSD`, is
lacking, although compilation and unit testing is known to work. A
page dedicated to `FreeBSD` is available [here](install-freebsd.html).

The installation on [Windows platform](http://windows.microsoft.com)
is described in the following pages:

- [Installation based on `cmake` and `Visual Studio`](install-windows-VisualStudio.html).
- [Installation based on `cmake` and `MinGW` (as packaged with `Cast3M` `2017`)](install-windows-Cast3M2017.html). This
  tutorial can easily be adapted to other versions of `MinGW`, without
  requiring `Cast3M` to be installed.
- [Installation on [Windows](http://windows.microsoft.com) in the [`MSYS2`](https://www.msys2.org/) environment](install-windows-msys.html).

The creation of binary packages are detailed [here](packages.html). 

A quick way of installing `TFEL` on `Ubuntu` systems is given in Section
@sec:QuickUbuntu.

`TFEL` is known to work on standard architectures implemented by the
Intel and AMD processors, either 32 or 64 bits. As no specific
instructions relative to the underlying architecture is used in the
code, other architectures shall work as well.

You must have the `cmake` build system (version greater than \(2.8\))
installed on your system, installing ̀`TFEL` basically boils down to the
following simple commands:

~~~~{#building1 .bash}
$ cmake [options]
$ make 
$ make install
~~~~

The optional parameters of `cmake` allows you to:

- specify the source location
- specify the installation directory
- *specify the interfaces to be build*

The rest of this document is dedicated to giving all the details
related to the installation process. For completeness, we will break
the installation procedure down into five steps:

- Checking for available prerequisites
- Downloading the source
- Configuration
- Building
- Testing (optional)
- Final install

# Prerequisites

## Compilers 

`TFEL` version 5.0 requires a `C++-20` compliant compiler, a `C`
compiler and optionally a `fortran` compiler. The following compilers
suite are officially supported:

- [The GNU Compiler Collection](https://gcc.gnu.org/). This is the
  default on most Linux distributions and is used for the development
  of `TFEL`.
- [The `clang` C and C++ compilers](http://clang.llvm.org/).
- The
  [Intel compilers suite](https://software.intel.com/en-us/c-compilers).

## Third party libraries

`TFEL` has been designed to have no dependencies to third parties
libraries with the very exception of the
[pybind11](https://github.com/pybind/pybind11) library used to create
optional bindings for the [`Python`](https://www.python.org/) language.

## Third party tools

The best way to build the `TFEL` is to use the
[`cmake`](http://www.cmake.org/) build system (see this
[section](#using-the-cmake-build-system)). *We only support `cmake`
versions greater than \(2.8\)*.

To build `TFEL` documentation, one may need:

- a valid [LaTeX](http://www.latex-project.org/) installation
  (reference manual in pdf format). The author uses the
  [texlive](https://www.tug.org/texlive/) distribution available with
  major Linux distributions.
- the [doxygen](http://www.stack.nl/~dimitri/doxygen/) tool (source 
  code documentation)

# Downloading the source

Official releases sources can be downloaded on [`TFEL` GitHub
page](https://github.com/thelfer/tfel/releases) (see the Download entry
of the navigation bar).

# Configuration

`TFEL` is based on the [`cmake`](http://www.cmake.org/) build-system.

## Creating a `build` directory

We highly recommend to use a separate directory to build the
sources.

In the following, we use the following convention:

- `<srcdir>` refers to the directory that actually contains the sources
  files.
- `<install_prefix>` refers to the final installation directory.

Both `<srcdir>` and `<install_prefix>` must contain an *absolute* path.

## Using the `cmake` build system

The use of the `cmake` build system is described in depth in the
`INSTALL-cmake` file that is located in the top directory of `TFEL`
sources. *We only support `cmake` versions greater than \(2.8\)*, so
please check the version available on your system:

~~~~ {#cmake-version .bash}
$ cmake --version
~~~~

A typical usage of `cmake` is the following:

~~~~ {#building .bash}
$ cmake <srcdir> -DCMAKE_BUILD_TYPE=Release -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-aster=ON -DCMAKE_INSTALL_PREFIX=<install_prefix>
~~~~

This will build `TFEL` with the support of interfaces for the
[Cast3M](http://www-cast3m.cea.fr/) and
[Code-Aster](http://www.code-aster.org) finite element solvers.

Various other options can be passed to `cmake`:

- The option `-Denable-cyrano=ON` enables the interface for the
  `Cyrano3` fuel performance code.
- To enable the mechanical behaviour interface for the ZeBuLoN finite
  element solver, one may use the `-Denable-zmat=ON
  -DZSET_INSTALL_PATH=$ZSETPATH` option, where `$ZSETPATH` points to
  the installation directory of the ZeBuLoN finite element solver.
- To enable the material properties interface for the
  [`Python`](https://www.python.org/) language, one may use the
  `-Denable-python=ON` option.
- To enable bindings for the [`Python`](https://www.python.org/)
  language, notably for the `MTest` tool, one may use the
  `-Denable-python-bindings=ON` option. This requires
  [pybind11](https://github.com/pybind/pybind11) to be available.

Some default compiler settings are detected by `cmake`, depending on
your system. To explicitly specify the compilers to be used, one may
define one of the following variables:

- `CXX` : name of the `C++` compiler
- `CC`  : name of the `C` compiler
- `FC`  : name of the `fortran` compiler
- `F77` : name of the `fortran` compiler (77 standard)

For example, we can use the following command to select the
[Intel compilers suite](https://software.intel.com/en-us/c-compilers):

~~~~ {#building-icpc .bash}
$ CXX=icpc CC=icc FC=ifort F77=ifort cmake <srcdir> -DCMAKE_BUILD_TYPE=Release -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-aster=ON -DCMAKE_INSTALL_PREFIX=<install_prefix>
~~~~

### Controlling the generation of the documentation

Generation of the documentation can be controlled by the following
boolean options:

- `enable-doxygen-doc`, which enables or disables the generation of the
  doxygen documentation (disabled by default)
- `enable-reference-doc`, which enables or disables the reference
  documentation generation (enabled by default if `latex` or `pandoc` is
  found)
- `enable-website`, which enables or disables the generation of the
  `TFEL` website (enabled by default if `pandoc` is found)

### Localisation of the libraries

Some `LiNuX` distributions install libraries in `lib64` on \(64\) bits
architectures and in `lib`. This can be changed by defining a
`LIB_SUFFIX` variable like this:

~~~~ {#lib-dir .bash}
$ cmake -DLIB_SUFFIX=64 ....
~~~~

### Appending the version number

The `TFEL_APPEND_VERSION` option will append the version number to the
names of:

- The executables.
- The libraries.
- The python modules. Note that, to comply with `python` restriction
  on modules' names, the characters `.` and `-` are replace by `_` and
  that only the first level modules are affected.
- The directories in the `share` folder.

The headers are installed in a subfolder named `TFEL-${TVEL_VERSION}`.

For example, if the `TFEL` version is `3.0.2-dev`, using
`TFEL_APPEND_VERSION` option will generate:

- The `mfront-3.0.2-dev` executable.
- The `libTFELMaterial-3.0.2-dev.so` library.
- The `mtest_3_0_2_dev` `python` module.
- The `tfel_3_0_2_dev.material` `python` module. In this case, the
  second level (`material`) is not affected.

This allows multiple executables to be installed in the same
directory.

### Specifying a version flavour

The `TFEL_VERSION_FLAVOUR` lets the user define a string that will be
used to modify the names of executables, libraries and so on (see the
previous paragraph for details).

For example, using `-DTFEL_VERSION_FLAVOUR=dbg` at the `cmake`
invocation, will generate an executable called `mfront-dbg`.

This option can be combined with the `TFEL_APPEND_VERSION` option.

### Selecting the `python` version

A specific `python` version can be selected by setting the
`Python_ADDITIONAL_VERSIONS`, as follows:

~~~~{.bash}
cmake ../master/ -Denable-python-bindings=ON -DPython_ADDITIONAL_VERSIONS=2.7 ...
~~~~

# Building

The configuration step created a compilation environment based on the
`make` tool.

To build `TFEL` libraries and binaries, just type:

~~~~ {#building-make .bash}
$ make
~~~~

To reduce compilation times, one may want to build `TFEL` libraries
and binaries in parallel. In this case, just type:

~~~~ {#building-make-j .bash}
$ make -j X
~~~~

`X` being the number of processors available.

# Testing (optional)

`TFEL` is delivered with many unit tests (more than 500 if you are
using the `cmake` build system and all the interfaces available). To
build them and execute them, just type:

~~~~ {#building-check .bash}
$ make check
~~~~

Again, compilation time can be reduced using the `-j` option of the
`make` command.

## Building the documentation

If a valid [LaTeX](http://www.latex-project.org) distribution is
available, reference manuals can be built through:

~~~~ {#building-pdf .bash}
$ make doc-pdf
~~~~

If the [doxygen](http://www.stack.nl/~dimitri/doxygen/) tool is
available, code source documentation can be built through:

~~~~ {#building-html .bash}
$ make doc-html
~~~~

If [`pandoc`](http://johnmacfarlane.net/pandoc/index.html) is build
and if you are using the `cmake` build system, the previous command
will also install a local version of the `TFEL` website.

~~~~ {#building-website .bash}
$ make website
~~~~

All the documentation will be built with the following command:

~~~~ {#building-html .bash}
$ make doc
~~~~

# Final install

To install `TFEL` binaries and libraries in the directory pointed by
`<install_prefix>`, just type:

~~~~ {#building-install .bash}
$ make install
~~~~

The previous command also installs the documentation if built.

# Scripts to define environment variables for `TFEL` to work properly

Depending on the system and compilation options, some of following
variables shall be set for `TFEL` to work properly after the
installation: `TFELHOME`, `PATH`, `LD_LIBRARY_PATH` and `PYTHONPATH`.

`TFEL` installs automatically the following files in the installation
directory:

- `<install_prefix>/share/tfel/env/env.sh` for `UNIX` systems and the
  `bash` shell. This file shall be used as follows:

  ~~~~{.sh}
  $ source <install_prefix>/share/tfel/env/env.sh
  ~~~~
- `<install_prefix>\share\tfel\env\env.ps1` for `PowerShell`
  shell under `Windows`. This file shall be used as follows:

  ~~~~{.sh}
  $ .\<install_prefix>\share\tfel\env\env.ps1
  ~~~~
- `<install_prefix>\share\tfel\env\env.bat` for the historical `cmd`
  shell under `Windows`. This file shall be used as follows:

  ~~~~{.sh}
  $ call <install_prefix>\share\tfel\env\env.bat
  ~~~~

> **Note**
>
> Those variables are not required if `TFEL` is installed
> system-wide (for instance in `/usr/local`) and that the `TFEL`'s
> binaries are not relocated.

# A Quick installation procedure on `Ubuntu` {#sec:QuickUbuntu}

The following steps show a quick way of installing `TFEL` on `Ubuntu`
systems. The configuration described in this section only compiles the
`generic` interface, which makes it suitable for use with solvers using the
[`MFrontGenericInterfaceSupport`
project](https://github.com/thelfer/MFrontGenericInterfaceSupport/issues),
e.g. with `OpenGeoSys`, `FEniCS`, `MoFEM`, etc...

The compilation requires that `gcc`, `git` and `python3` are installed,
as well as the python module `numpy` and the `pybind11` library to build
the `python` bindings. Those packages can be installed using:

~~~~{.sh}
$ sudo apt install python3 python3-numpy pybind11-dev cmake g++ gfortran
~~~~

The next step is to get the `TFEL` source code:

~~~~{.sh}
$ cd <where_you_want_to_download_the_source>
$ mkdir -p TFEL && cd TFEL
$ git clone https://github.com/thelfer/tfel.git
~~~~

This will download the current development version of `TFEL`.

Then we create a `build` directory and call `ccmake`.

~~~~{.sh}
$ mkdir build && cd build
$ ccmake ../tfel
~~~~

`ccmake` allows to select various options. In this tutorial, we choose
the following options:

~~~~{.sh}
  CMAKE_INSTALL_PREFIX    ~/.local
  TFEL_APPEND_VERSION           ON
  enable-numpy-support          ON
  enable-python                 ON
  enable-python-bindings        ON
~~~~

The `TFEL_APPEND_VERSION` allows to install different versions of `TFEL`
at the same location.

Then configure twice `[c]`,`[c]` and generate `[g]`. Once done, just
type:

~~~~{.sh}
$ make
$ make install
~~~~

Finally, add the paths in your local `.bashrc` file:

~~~~{.sh}
export PATH=$PATH:~/.local/bin
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:~/.local/lib
export PYTHONPATH=${PYTHONPATH}:~/.local/lib/python3.8/site-packages
~~~~

Now you can use the local `TFEL` binaries such as `mfront-X.Y.Z-dev` and
`mtest-X.Y.Z-dev` or define some
global alias names for them.

<!-- 
# Windows MINGW
-->

<!-- Local IspellDict: english -->

