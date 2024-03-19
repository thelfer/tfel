---
title: Release notes of the 5.0 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer, Maxence Wangermez
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
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

# New `TFEL/Math` features

## tiny matrices product

The product of two tiny matrices has been implemented:

~~~~{.cxx}
const auto m1 = tmatrix<2u, 2u, int>{0, 1,  //
                                     2, 3};
const auto m2 = tmatrix<2u, 2u, int>{4, -1,  //
                                     5, 2};
const auto m3 = m1 * m2;
~~~~
