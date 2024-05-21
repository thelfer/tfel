---
title: Using the `TFEL` libraries in `C++`
author: Thomas Helfer, Maxence Wangermez
date: 2024
lang: en-EN
numbersections: true
documentclass: article
from: markdown+tex_math_single_backslash
geometry:
  - margin=2cm
papersize: a4
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

The `TFEL` project generates several `C++` libraries that can be used
directly: `TFELMFront`, `TFELMTest`, `TFELSystem`,
[`TFELMaterial`](tfel-material.html), `TFELMathCubicSpline`,
`TFELMathKriging`, `TFELMathParser`, [`TFELMath`](tfel-math.html),
`TFELTests`, `TFELCheck`, `TFELException`, `TFELGlossary`,
`TFELUtilities`, `TFELNUMODIS`, `TFELConfig`, and `TFELUnicodeSupport`.

Using those libraries requires in practice to add several pre-processor,
compiler or linker flags, including paths to headers and libraries, and
handling dependencies between those libraries.

The `TFEL` project provides two tools to ease developers' life:

- the `tfel-config` utility,
- a set of [`cmake`](https://cmake.org/) packages.

# Using `tfel-config`

The `tfel-config` utility provides many informations useful for
compiling projects based on the `TFEL` libraries. This utility is used
internally by `MFront` (see [this
page](compiling-mfront-shared-libraries.html) for details).

## Paths

Most paths are influenced by the `TFELHOME` environment variable. If this
variable is not defined, then the installation path, defined by the
`CMAKE_INSTALL_PREFIX` variable, is used.

### Headers

The following flags allow to get the paths toward the `TFEL` headers.

- `--include-path`: returns the path to the `TFEL` headers.
- `--includes`: returns pre-processor directives to include the `TFEL`
  headers.

### Libraries

The following flags allow to request information for the linker:

- `--library-path`: returns the path to the `TFEL` libraries.
- `--libs`: returns linker directives to link with the selected libraries
  (see below). This option also returns the linker directives for the
  dependencies of the selected libraries. 

## Compilation flags

The following flags allow to request various pre-processor, compiler or
linker flags:

- `--compiler-flags` : returns TFEL's recommended compiler flags.
- `--cppflags` : returns flags related to the `C++` standard used.
- `--cxx-standard` : prints the version of the `C++` standard used to
  compile TFEL.
- `--debug-flags` : returns flags adding debugging symbols.
- `--ldflags` : returns linking flags.
- `--oflags` : returns tfel recommended optimisation flags with
  architecture specific flags.
- `--oflags0` : returns tfel recommended optimisation flags without
  architecture specific flags.
- `--oflags2` : returns some aggressive optimisation flags, possibly at
  the expense of numerical precision. This shall be added to `--oflags`
  results.

Note that those flags are consistent with the compiler used to compile
the `TFEL` project.

## Selection of the libraries

The following flags allow to select specific libraries:

- `--material`         : requests flags for the `TFELMaterial` library.
- `--math`             : requests flags for the `TFELMath` library.
- `--math-cubic-spline`: requests flags for the `TFELMathCubicSpline` library.
- `--math-kriging`     : requests flags for the `TFELMathKriging` library.
- `--math-parser`      : requests flags for the `TFELMathParser` library.
- `--numodis`          : requests flags for the `TFELNUMODIS` library.
- `--exceptions`       : requests flags for the `TFELException` library.
- `--glossary`         : requests flags for the `TFELGlossary` library.
- `--mfront-profiling` : requests flags for the `libMFrontProfiling` library.

## Examples of usage

The following examples showcase typical usages of `tfel-config`:

~~~~{.bash}
$ tfel-config --includes
-I<TFEL_INSTALL_PATH>/include 
$ tfel-config --include-path
<TFEL_INSTALL_PATH>/include 
$ tfel-config --libs --math-parser
-L<TFEL_INSTALL_PATH>/lib -lTFELMathParser -lTFELMathKriging -lTFELMath -lTFELUnicodeSupport -lTFELException
~~~~

In those example, `<TFEL_INSTALL_PATH>` is to be replaced by the actual
installation path of the `TFEL` project.

# Using `cmake`

The following `cmake` packages are available: `TFELMFront`, `TFELMTest`,
`TFELMaterial`, `TFELMath`, `TFELMathCubicSpline`, `TFELMathKriging`,
`TFELMathParser`, `TFELSystem`, `TFELNUMODIS`, `TFELCheck`,
`TFELUtilities`, `TFELGlossary`, `TFELTests`, `TFELUnicodeSupport`,
`TFELException`, `TFELConfig`

Those packages can be loaded using the `find_package` command. It is
highly recommended to pass the `TFEL_DIR` to `cmake` to indicate where
the `TFEL` library has been installed.

Note that those packages import targets in the `tfel::` namespace.

## Example of usage

The following `CMakeLists.txt` file declares a simple executable relying
the `TFELMathCubicSpline` library:

~~~~{.cmake}
project(Test)
cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
find_package(TFELMathCubicSpline REQUIRED HINTS "${TFEL_DIR}/share/tfel/cmake")
add_executable(test main.cxx)
target_link_libraries(test tfel::TFELMathCubicSpline)
~~~~
