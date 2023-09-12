---
title: Release notes of the 4.2 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer
date: 2022
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

# Issues fixed

## Issue #427: [mtest] Add support for integral tests based on the inner pressure or the axial force in the ptest scheme

When the evolution of the inner radius or the evolution of the outer
radius is prescribed, integral tests on the inner pressure
(InnerPressure`) can be set.

When the evolution of the axial growth is prescribed, integral tests on
the axial force (`AxialForce`) can be set.

For more details, see <https://github.com/thelfer/tfel/issues/427>

## Issue #426: [mtest] Add support for tests based on analytical formulae in the ptest scheme mtest

The `@Test` keyword now supports the `function` option, the syntax of
which is similar to the `@Test` keyword of the `mtest` scheme.

For example, the following line compares the value of the inner pressure
to the values of an evolution named `InnerPressureReferenceValues`:

~~~~{.cxx}
@Test<function> 'InnerPressure' 'InnerPressureReferenceValues' 1e2;
~~~~

For more details, see <https://github.com/thelfer/tfel/issues/426>

## Issue #425: [mfront] query the `mdnx` path where an `OverridableImplementation` is written

The `getDestinationPathInMadnexFile` returns the `mdnx` path where an
`OverridableImplementation` is written.

For more details, see <https://github.com/thelfer/tfel/issues/425>

## Issue #424: [madnex] Add an option to use a template file when generating a `mdnx` file

For more details, see <https://github.com/thelfer/tfel/issues/424>

## Issue 422: Document the product of two symmetric tensors and its derivatives

For more details, see <https://github.com/thelfer/tfel/issues/422>

## Issue 383: [python-bindings] Add the ability to retrieve the `TFEL` version

For more details, see <https://github.com/thelfer/tfel/issues/383>

## Issue 369: [TFEL/Utilities] support for raw string literals in `CxxTokenizer`

For more details, see <https://github.com/thelfer/tfel/issues/369>

## Issue 362: [mfront] Better code generation for point-wise models using the Cast3M interface 

For more details, see <https://github.com/thelfer/tfel/issues/362>

## Issue 360: [mtest] Documentation of the  `@Profile` keyword in `PTest`

For more details, see <https://github.com/thelfer/tfel/issues/360>

## Issue 356:  [mfront] modification of the Gurson-Tvergaard-Needleman stress criterion

For more details, see <https://github.com/thelfer/tfel/issues/356>

## Issue 353: [mfront] Add support for unsymmetric tensorial variables when generating `MTest` files

For more details, see <https://github.com/thelfer/tfel/issues/353>
