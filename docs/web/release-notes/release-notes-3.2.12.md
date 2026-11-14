---
title: Release notes of the 3.2.12 version of `TFEL`, `MFront` and `MTest`
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

The page describes the new functionalities of Version 3.2.12 of the
`TFEL` project.


This version was released on May 12, 2025 along with:

- [Version 3.0.15](release-notes-3.0.15.html),
- [Version 3.1.15](release-notes-3.1.15.html),

and inherits from all fixes from those releases.


# Issues fixed

## Issue 771:[Hooke stress potential] The initial_geometry_reference_temperature is not listed in the authorized options of the potential

For more details, see <https://github.com/thelfer/tfel/issues/771>.

## Issue 765: [mfront] Fix numerical tests in the `Swift` and `Power` hardening rules

For more details, see <https://github.com/thelfer/tfel/issues/765>.

## Issue 705: [mfront] fix the computation of the derivative of some isotropic hardening rules

The corrected hardening rules did not take into account a theta factor
in the expression of the derivative of the elastic limit, which could
affect convergence when this parameter differs from `1`.

For more details, see <https://github.com/thelfer/tfel/issues/705>.
