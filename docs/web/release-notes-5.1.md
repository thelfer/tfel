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
`unversioned-python-module-names` option to `OFF`.

# `tfel-config`

On `Windows`, the `--registry-key` option returns the registry key which
is used to determine the TFEL's installation path.

The `--tfel-home` option returns the environment variable in which the
TFEL's installation path shall be defined. 

## Example of usage

~~~~
$ tfel-config-5.1.0-dev-release --tfel-home
TFELHOME_5_1_0_dev_release
~~~~

# Python bindings

Python bindings are now generated using the
[`pybind11`](https://github.com/pybind/pybind11) library.

# Issues fixed

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

