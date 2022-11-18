---
title: Release notes of the 3.3.4 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer
date: 14/01/2022
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

Version 3.3.4 has been released on October, 24th 2022. It inherits from
all the issues fixes in:

- [Version 3.0.11](release-notes-3.0.11.html).
- [Version 3.1.11](release-notes-3.1.11.html).
- [Version 3.2.8](release-notes-3.2.8.html).


# Issues fixed

## Issue 322: [cyrano interface] The output status does not match the specification when a variable is out of its bounds

For more details, see <https://github.com/thelfer/tfel/issues/322>

## Issue 268: Python binding of ExternalMaterialPropertyDescription class: error if no input variable in the MFront 

For more details, see <https://github.com/thelfer/tfel/issues/268>

## Issue 142: [mfm-test-generator] handing of evolutions is broken in MTestInputFileGenerators for evolutions defined by a dictionnary

For more details, see <https://github.com/thelfer/tfel/issues/142>

## Issue 141: [mtest] ThermalExpansion is not property handled in the list of optional material properties by the Cast3MStandardBehaviour class for an isotropic behaviour

For more details, see <https://github.com/thelfer/tfel/issues/141>

## Issue #80: [mfront] The variable seq is defined twice by the `MohrCoulomb` stress criterion in case of non-associated flow

Thig bug was only triggered in the `3.3.x` series if `C++-17` support was enabled.

For more details, see <https://github.com/thelfer/tfel/issues/80>
