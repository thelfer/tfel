---
title: A description of the compilation process used by `MFront` and the available options to control this process
author: Thomas Helfer, Maxence Wangermez
date: 1/07/2020
lang: en-EN
link-citations: true
colorlinks: true
geometry:
  - margin=2cm
papersize: a4
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

# General overview

`mfront` handles the compilation of shared libraries. More precisely,
the compilation phase is delegated to external tools called generators
in the `mfront` wording. `mfront` currently supports two generators:
`make` and `cmake`.

In both cases, the compilation process can be affected by defining any
of the following *optional* environment variables:

- `TFELHOME`: this variable must contain the path where `mfront` is
  installed. This variable is used by `tfel-config` to return the path
  to the header files and libraries delivered by the TFEL
  project. This variable thus affects the output of the following
  commands (see `tfel-config --help` for details):
  - `tfel-config --includes` and `tfel-config --include-path`
  - `tfel-config --libs` and `tfel-config --library-path`
- `CXX`: this variable defines the `C++` compiler to be used. Note
  that this choice must be consistent with the compiler used to
  compile `mfront` if the compiler flags defined by default by
  ``mfront` are used (see below).
  
- `CXXFLAGS`: this variable defines the compiler flags to be used. If
  this variable is not defined, `mfront` will use the following outputs
  of `tfel-config` (see `tfel-config --help` for details): `tfel-config
  --warnings --compiler-flags --cppflags --cxx-standard`
- `LDFLAGS`: this variable defines flags passed to the linker.

Those compiler flags are completed by the following outputs of
`tfel-config` depending on the level of optimisation requested by the
user using the `--obuild` command line option of `mfront`:

- with `--obuild=level0`: `tfel-config --oflags0`
- with `--obuild=level1` (or simply `--obuild`): `tfel-config --oflags`
- with `--obuild=level2`: `tfel-config --oflags --oflags2`

If the `-g` command line option is passed to `mfront`, those flags are
completed by the following output of `tfel-config`: `tfel-config
--debug-flags`.

# Variables affecting the `make` generator

- `MAKE`:  executable to be used 

# Variables affecting the `cmake` generator

- `CMAKE`: executable to be used 
- `CMAKE_GENERATOR`: name of the `cmake`' build system generatorto be
  used.

