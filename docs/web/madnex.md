---
title: Support of `madnex` files in `MFront`
author: Thomas Helfer
date: 2020
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
tabPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
bibliography: bibliography.bib
---

The `madnex` file format has been developed by `EDF` to store their
experimental data. It extends the `HDF5` file format by providing a
suitable data model. Internally, `EDF` uses a closed-source `python`
library to insert, query and extract data from `madnex` file.

The `madnex` library is an open-source `C++` library which currently
focus on the storage of `MFront` implementations. This library can be
downloaded here:

<https://github.com/thelfer/madnex>

The website of this library [@cea_edf_madnex_library_2020] details the
data model used to store `MFront` implementations [@].

# Compilation of `TFEL` with `madnex` files support

By default, support of `madnex` files is disabled.

Support for `madnex` files as inputs of `MFront` must be explicitly
enabled during the compilation of the `TFEL` project. More precisely,
when invoking the [`cmake`](https://cmake.org/) build system, the
following variables must be set:

1. `enable-madnex` must be set to `ON`.
2. `Madnex_DIR` must be set to the path where the `MadnexConfig.cmake`
  file has been installed. This path is usually given by
  `$MADNEXHOME/share/madnex/cmake/` where `MADNEXHOME` is the
  installation directory of the `madnex` library.

Hence, a typical call `cmake` to cmake would be similar as:

~~~~{.bash}
$ cmake -Denable-madnex=ON -DMadnex_DIR=$MADNEXHOME/share/madnex/cmake ...
~~~~

This call assumes that that the `MADNEXHOME` environment variable has
been set.

More information about the options that can be passed to `cmake` are
given in the `INSTALL-cmake` file which is located at the root of the
`TFEL` sources.

# `mfront-query` usage

## Options specific to `madnex` files

Several options has been added to the `mfront-query` specifically for
`madnex` files.

The `--list-materials` returns the list of all materials in a `MFront`
file:

~~~~{.bash}
$ `mfront-query` --list-materials file.madnex
~~~~



~~~~{.bash}
$ `mfront-query` --list-behaviours file.madnex
~~~~

~~~~{.bash}
$ `mfront-query` --material=<material_id> --list-behaviours file.madnex
~~~~

## Options specific to `madnex` files

# `mfront` usage

~~~~{.bash}
$ mfront --obuild --interface=generic --material=<material_id> --behaviour=<behaviour_id> file.madnex
~~~~

The '--behaviour' option is not mandatory when only one behaviour is
stored.

~~~~{.bash}
$ mfront --obuild --interface=generic --material=<material_id> --all-behaviours file.madnex
~~~~

~~~~{.bash}
$ mfront --obuild --interface=generic --all-behaviours file.madnex
~~~~

# `mfront` python module

<http://tfel.sourceforge.net/mfront-python.html>

~~~~{.python}
import mfront

dsl = mfront.getDSL("file.madnex",<material_id>,<behaviour_id>)
dsl.analyseFile("file.madnex",<material_id>,<behaviour_id>)
~~~~

~~~~{.python}
import mfront

dsl = mfront.getDSL("file.madnex",<behaviour_id>)
dsl.analyseFile("file.madnex",<behaviour_id>)
~~~~

# References