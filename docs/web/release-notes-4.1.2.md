---
title: Release notes of the 4.1.2 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer
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

The page describes the new functionalities of Version 4.1.2 of the
`TFEL` project.

Version 4.1.2 has been released on June, 4th 2024. This version
inherits from the bug fixes of:

- Version [3.0.13](release-notes-3.0.13.html),
- Version [3.1.13](release-notes-3.1.13.html),
- Version [3.2.10](release-notes-3.2.10.html),
- Version [3.3.5](release-notes-3.3.5.html),
- Version [3.4.6](release-notes-3.4.6.html),
- Version [4.0.3](release-notes-4.0.3.html).


# Issues fixed

## Issue 548: [generic-interface] Fix initialisation of array of material properties when quantities are used

For more details, see <https://github.com/thelfer/tfel/issues/548.

## Issue 541: [tfel-math] support for quantities in `computePushForwardDerivative` and `convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative`

For more details, see <https://github.com/thelfer/tfel/issues/540.

## Issue 540: Allow behaviour to call models with more than one output

For more details, see <https://github.com/thelfer/tfel/issues/540.

## Issue 519: [ImplicitCZM] Typo in ComputeCohesiveForce block

For more details, see <https://github.com/thelfer/tfel/issues/519.

## Issue 514: [material-properties] add try/catch blocks in interfaces for the C family (fortran, C, Cast3M)

For more details, see <https://github.com/thelfer/tfel/issues/514.
