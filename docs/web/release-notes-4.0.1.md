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

