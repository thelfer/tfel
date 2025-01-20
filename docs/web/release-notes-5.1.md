---
title: Release notes of the 5.1 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer, Antoine Martin
date: 2025
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

# Known incompatibilities

When compiling with option `TFEL_APPEND_VERSION` set to `ON` or when
defining the string variable `TFEL_VERSION_FLAVOUR`, the `python`
modules are now modified to reflect those information. This old
behaviour can be restored by setting the
`unversioned-python-module-names` option to `ON`.

# New features in the `TFEL` libraries

## Environment

This version now supports a new environment variable to specify the
`TFEL` installation directory that always supersedes the `TFELHOME`
environment variable. The name of this variable depends on the version
of the project, the fact that a development version has been compiled
and the flavour (optionally specified with the `cmake` variable
`TFEL_VERSION_FLAVOUR`). The name of this variable can be retrieved with
the `--tfel-home` option of `tfel-config` described below.

## `tfel-config`

### `--registry-key` option

On `Windows`, the `--registry-key` option returns the registry key which
is used to determine the TFEL's installation path.

### `--tfel-home` option

The `--tfel-home` option returns the environment variable in which the
TFEL's installation path shall be defined. 

#### Example of usage

~~~~
$ tfel-config-5.2.0-dev-release --tfel-home
TFELHOME_5_2_0_dev_release
~~~~

### `--python-module-suffix` option

This option return the suffix of the `python` module. Such suffix is not
empty only the following conditions are met:

- `TFEL` has been compiled with option `TFEL_APPEND_VERSION` set to `ON` and or
  with the string variable `TFEL_VERSION_FLAVOUR` defined,
- the `unversioned-python-module-names` option has been set to `ON` (the
  default value if `OFF`).

The option `--python-module-suffix` is only available if the python
bindings is available.

#### Example of usage

~~~~
$ tfel-config-5.1.0-release --python-module-suffix
5_1_0_release
~~~~

# New `TFEL/Material` features

## Homogenization

### Ellipsoidal inclusion embedded in anisotropic matrix

When \(\tenseur C_0\) is anisotropic, the Eshelby tensor can be computed
with `computeAnisotropicEshelbyTensor` in 3D and `compute2DAnisotropicEshelbyTensor`
in 2D. There are also `computeAnisotropicHillTensor`, `compute2DAnisotropicHillTensor`,
and also `computeAnisotropicLocalisationTensor` and `compute2DAnisotropicLocalisationTensor`.

### Homogenization bounds

Different homogenization bounds are implemented.
The available functions are `computeVoigtStiffness`, `computeReussStiffness`.



# Python bindings

Python bindings are now generated using the
[`pybind11`](https://github.com/pybind/pybind11) library.

# Issues fixed

## Issue 677:[tfel-config] add a python-module-suffix option

For more details, see <https://github.com/thelfer/tfel/issues/677>

## Issue 676: Always prefer the versioned `TFELHOME` environment variable

For more details, see <https://github.com/thelfer/tfel/issues/676>

## Issue 674: [python-bindings] Add option to disable the modication of the names of  python modules by the `TFEL_APPEND_VERSION` and `TFEL_VERSION_FLAVOUR`

For more details, see <https://github.com/thelfer/tfel/issues/674>

## Issue 673: [tfel-config] Add the ability to define an environment variable associated with the `TFEL`'s version and flavour

For more details, see <https://github.com/thelfer/tfel/issues/673>

## Issue 672: [python bindings] fix python modules names when `TFEL_APPEND_VERSION` or `TFEL_VERSION_FLAVOUR` is defined

For more details, see <https://github.com/thelfer/tfel/issues/672>

## Issue 667: Reduce the size of files paths in sources to fit in `Windows`'s `MAX_PATH` limit

For more details, see <https://github.com/thelfer/tfel/issues/667>

## Issue 666: [doc] Add CONTRIBUTING file, remove devel.md

For more details, see <https://github.com/thelfer/tfel/issues/666>

## Issue 654: Add PowerShell environment script

For more details, see <https://github.com/thelfer/tfel/issues/654>

## Issue 294: [python-bindings] Evaluate port to `pybind`

For more details, see <https://github.com/thelfer/tfel/issues/293>

