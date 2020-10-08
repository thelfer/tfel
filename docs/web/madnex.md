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
data model used to store `MFront` implementations.

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

# `mfront` usage

The only thing specific to `madnex` files is the selection of the
implementation to be treated.

For a behaviour, this selection is done through the `--material` and
`--behaviour` options. For example:

~~~~{.bash}
$ mfront --obuild --interface=generic --material=<material_id> --behaviour=<behaviour_id> file.madnex
~~~~

Here, `behaviour_id` can be a regular expression.

For material properties and models, the `--material-property` and
`--model` options have a similar role than the `--behaviour` option for
behaviours.

The `--all-material-properties`, `--all-behaviours`, and `--all-models`
options allows to select all the material properties, behaviours and
models respectively, as follows:

~~~~{.bash}
$ mfront --obuild --interface=generic --material=<material_id> --all-behaviours file.madnex
~~~~

The material name `<none>` has a special meaning here: it allows the
selection of implementations that not associated with a material.

# `mfront` python module

The `mfront` python module is fully described here:

<http://tfel.sourceforge.net/mfront-python.html>

The main function related to `madnex` file support is the
`getImplementationsPaths` which takes five arguments:

- the name of the file. The name of this argument is `file`;
- the name of a material. If the material name is empty, the
  implementations associated with all the materials are are considered
  and also the implementations associated with no material. To select
  only implementations associated with no material, one shall use the
  special material name `<none>`. The name of this argument is `material`.
- a regular expression describing the searched material properties. This
  argument can be empty. The name of this argument is
  `material_property`.
- a regular expression describing the searched behaviours. This argument
  can be empty. The name of this argument is `behaviour`.
- a regular expression describing the searched models. This argument can
  be empty. The name of this argument is `model`.

This function returns a list of strings where the path of the
implementation is encoded.

~~~~{.python}
import mfront

paths = mfront.getImplementationPaths("file.madnex",
                                      material=<material_id>,
                                      behaviour=behaviour_id>)
if len(paths) == 0:
  raise("no matching implementation")

dsl = mfront.getDSL(paths[0])
dsl.analyseFile(paths[0])
~~~~

# `mfront-query` usage

## Options specific to `madnex` files

Several options has been added to the `mfront-query` specifically for
`madnex` files.

Queries of the file itself (list of materials, behaviours, etc...) are
also provided by the `madnex-query` tool.

### Listing all material a `madnex` file

The `--list-materials` returns the list of all materials in a `MFront`
file:

~~~~{.bash}
$ `mfront-query` --list-materials file.madnex
~~~~

### Listing of all material properties, all behaviours or all models

The following options are available:

- `--list-material-properties`: list of material properties.
- `--list-behaviours`: list of all behaviours.
- `--list-models`: list of all models.

A typical call is as follows:

~~~~{.bash}
$ `mfront-query` --list-behaviours file.madnex
~~~~

The previous options are affected by the the definition of a material,
as follows:

~~~~{.bash}
$ `mfront-query` --material=<material_id> --list-behaviours file.madnex
~~~~

## Querying information about implementation, options specific to `madnex` files

For a behaviour, the selection of an implementation on which the queries
are made is done through the `--material` and `--behaviour` options. For
example:

~~~~{.bash}
$ mfront-query --obuild --interface=generic --material=<material_id> --behaviour=<behaviour_id> file.madnex --state-variables
~~~~

Here, `behaviour_id` can be a regular expression.

For material properties and models, the `--material-property` and
`--model` options have a similar role than the `--behaviour` option for
behaviours.

# Storing an `MFront` file in a `madnex` file

The easiest way to store an `MFront` file is to create an instance of
the `OverridableImplementation` class and then call the `write`
function. Here is an example in `python`:

~~~~{.python}
import mfront

i = mfront.OverridableImplementation("Plasticity.mfront")
mfront.write(i, "Plasticity.madnex")
~~~~


However, the `OverridableImplementation` class also allows modifying
the medata of a implementation, as follows:

~~~~{.python}
import mfront

i = mfront.OverridableImplementation("Plasticity.mfront")
i.overrideBehaviourName("Plasticity_SRMA2022")
i.overrideMaterial("A316LN")
i.overrideAuthor("John Mac Enroe")
mfront.write(i, "Plasticity.madnex")
~~~~

# References
