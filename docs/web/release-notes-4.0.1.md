---
title: Release notes of the 4.0.1 version of `TFEL`, `MFront` and `MTest`
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

## Issue #105: [TFELMath] Missing specialisations of the `DerivativeTypeDispatcher` class

Many specialisations of the `DerivativeTypeDispatcher` class are missing:

- specialisations for the derivatives of a `tvector` with respect to a scalar
- specialisations for the derivatives of a `tmatrix` with respect to a scalar
- specialisations for the derivatives of a `tvector` with respect to a `tvector`
- etc...

For more details, see <https://github.com/thelfer/tfel/issues/105>

## Issue #102: [mfront] Missing `using namespace std;` in the body of the `computeFlow` method of the `IsotropicStrainHardeningMisesCreep` DSL

For more details, see <https://github.com/thelfer/tfel/issues/102>

## Issue #101: [mfront] [generic interface] The computation of stored and dissipated energies fails to compile if quantities are used bug generic interface mfront

For more details, see <https://github.com/thelfer/tfel/issues/101>

## Issue #81: [TFELMaterial] The Mohr-Coulomb stress criterion is broken after pull request `#5`

For more details, see <https://github.com/thelfer/tfel/issues/81>

## Issue #78: [TFELMath] missing `const` qualifier in the implementation of the `map` function for immutable views

For more details, see <https://github.com/thelfer/tfel/issues/78>

## Issue #77: [mfront] The type `TypeParsingOptions` is not explicitely exported which leads to an error in `gcc` under `Windows`

For more details, see <https://github.com/thelfer/tfel/issues/77>

## Issue #56: [TFELMath] The CublicSpline methods do not compile if TFEL/Math/vector.hxx is included

For more details, see <https://github.com/thelfer/tfel/issues/56>

## Issue #54: [mfront] Inconsistent declaration of array of parameters as `tvector`

`TFEL-4.0` changed the type of all array to fsarray, except for arrays of
parameters.

For more details, see <https://github.com/thelfer/tfel/issues/54>.

## Issue #53: [mfront] HarmonicSumOfNortonHoffViscoplasticFlows is broken in `TFEL-4.0`

For more details, see <https://github.com/thelfer/tfel/issues/53>.

## Issue #37: [mfront] `@ElasticMaterialProperties` is broken when using quantities

For more details, see <https://github.com/thelfer/tfel/issues/37>.

