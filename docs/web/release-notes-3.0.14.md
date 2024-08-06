---
title: Release notes of the 3.0.14 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer
date: 6/12/2023
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

The page describes the new functionalities of Version 3.0.14 of the
`TFEL` project.

# Issues fixed

## Issue 605: update `octave` interface

For more details, see <https://github.com/thelfer/tfel/issues/605>.

## Issue 583: [mtest] Fix typo when comparing tangent operator to its numerical approximation

For more details, see <https://github.com/thelfer/tfel/issues/583>.

## Issue 580: [aster interface] Some fields are missing in the default implementation of the `AsterTraits` class

The following fields are missing:

~~~~{.cxx}
static constexpr unsigned short material_properties_nb = 0u;
static constexpr unsigned short elasticPropertiesOffset = 0u;
static constexpr unsigned short thermalExpansionPropertiesOffset = 0u;
~~~~

For more details, see <https://github.com/thelfer/tfel/issues/580>.
