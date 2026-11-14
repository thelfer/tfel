---
title: Release notes of the 3.0.10 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer
date: 21/09/2021
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

Version 3.0.10 was released on October, 26th 2021.

# Issues fixed

## Issue #17: [mtest]  Unsupported degenerated case when defining the rotation matrix using Miller' indices

For more details, see <https://github.com/thelfer/tfel/issues/17>.

## Issue #16: [mtest] Unnecessary  brackets when declaring rotation matrix using Euler conventions

For more details, see <https://github.com/thelfer/tfel/issues/16>.

## Issue #14: [aster interface] The 3D modelling hypothesis must always be defined

Behaviours restricted to 2D hypotheses could not compile with the
`aster` interface as the 3D modelling hypothesis was always expected to
be supported.

For more details, see <https://github.com/thelfer/tfel/issues/14>.

