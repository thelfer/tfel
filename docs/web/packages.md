---
title: Creating packages for TFEL/MFront
author: Thomas Helfer
date: October 31, 2014
lang: en-EN
---

The latest version of this document is always available at
<https://thelfer.github.io/tfel/web/packages.html>.

This document describes how to generate packages for `TFEL`. It relies
on the instructions given [here](install.html) to build `TFEL` from
the sources using the `cmake` build system.

# Building portable binaries

The section is only meaningful if you wish to share the generated
packages between various computers: it can be skipped if you only want
to generate a package for you personal use.

## The meaning of portable

By portable, we mean binaries that can be executed on machines:

- sharing the same architecture (namely \(32\) or \(64\) bits), but
  not necessarily the same processor versions: we will exclude from
  the compilation all flags leading to specific processor
  instructions, such as the `sse` instructions.
- the same environment. This most notably means:
 - the same compiler version. The reason for this restriction is
   explained below.
 - the same version of `python` if the `python` bindings are enabled.

However, using portable binaries does not affect the fact that
`MFront` will try to generate the most effective code for you
particular machine *once installed*. We rely on the fact that most
compilers have an option to detect the current machine processor: for
`gcc` and `clang` this option is `-march=native`. This option is still
enabled when compiling libraries generated by `MFront` when using the
`--obuild` option. This is why it is preferable to use portable
binaries with the same compiler version that the one use to produce
those binaries.

## Special configuration option

We introduced an additional option to `cmake` named
`enable-portable-build` to allow building a portable version of
`TFEL`. This option must be added to the `cmake` command line:

~~~~ {#building1 .bash}
$ cmake [options] -Denable-portable-build=ON
~~~~~~~~~~~~~~~~~~~~~~

The various other options that can be passed to `cmake` are described
[here](install.html).

# Generating packages

## Basic steps

~~~~ {#building2 .bash}
$ cmake [options] -Denable-portable-build=ON
$ make
$ cpack -G [GENERATOR_NAME]
~~~~

The list of available generators are given by the `cpack --help`
command:

~~~~ {#generators .bash}
$ cpack --help
~~~~~~~~~~~~~~~~~~~~~~

On `Debian wheezy`, the following generators are avaiable:

- `DEB` : Debian packages
- `NSIS` : Null Soft Installer
- `RPM` : RPM packages
- `STGZ` : Self extracting Tar GZip compression
- `TBZ2` : Tar BZip2 compression
- `TGZ` : Tar GZip compression
- `TZ` : Tar Compress compression
- `ZIP` : ZIP file format

## List of components

Depending on the options provided to the `cmake` command and the
generator used, various packages can be build:

- `core` contains all the core `TFEL` libraries.
- `mfront` contains the `MFront` code generator.
- `mtest` contains the `MTest` solver.
- `docs` contains the reference documentation (for `TFEL`, `MFront`
  and `MTest`)
- `doxygen` contains the `doxygen` documentation (for `TFEL`, `MFront`
  and `MTest`).
- `python_bindings` contains the `python` bindings (for `TFEL`, `MFront`
  and `MTest`).

## Building debian packages

~~~~ {#generator_deb .bash}
$ cmake ../trunk -DCMAKE_BUILD_TYPE=Release -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-python=ON -Denable-python-bindings=ON -Denable-aster=ON -Denable-zmat=OFF -Denable-cyrano=ON -Denable-portable-build=ON
$ cpack -G DEB
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Building `RPM` packages for `Mageia` (\(64\) bits)

~~~~ {#generator_rpm .bash}
$ cmake ../trunk -DCMAKE_BUILD_TYPE=Release -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-python=ON -Denable-python-bindings=ON -Denable-aster=ON -Denable-zmat=OFF -Denable-cyrano=ON -Denable-portable-build=ON -DLIB_SUFFIX=64
$ cpack -G RPM
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The `LIB_SUFFIX` is used to specify that the libraries must be place
in the `lib64` directory.

## Building `NSIS` for `Windows` on `LiNuX`

If you have a cross-compiler (in the case of your example, `mingw64`)
and the `nsis` package installed, one can build a `Windows` package
like this:

~~~~ {#generator_NSIS .bash}
$ cmake ../trunk -DCMAKE_BUILD_TYPE=Release -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-python=OFF -Denable-python-bindings=OFF -Denable-aster=ON -Denable-zmat=OFF -Denable-cyrano=ON -DCMAKE_TOOLCHAIN_FILE=../trunk/cmake/ToolChain-i686-w64-mingw32.cmake -DCMAKE_INSTALL_PREFIX=/home/th202608/codes/tfel/tfel-2.0.x/install -Denable-portable-build=ON -Denable-static=ON
$ cpack -G NSIS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The `enable-static` portion is mandatory since the shared libraries
associated with the cross compiler won't be present on `Windows`.

One may use another cross-compiler by providing an appropriate
tool-chain file.

## 

<!-- Local IspellDict: english -->
