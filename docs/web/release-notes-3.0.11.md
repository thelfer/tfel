---
title: Release notes of the 3.0.11 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer
date: 16/11/2021
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

## Issue #29: [TFELMath] Fix definition of reference in DifferenceRandomAccessIterator

For more details, see <https://github.com/thelfer/tfel/issues/21>.

## Issue #21: [TFELMath] Fix support for negation of tensors based on quantitites

The following code did not compile:

~~~~{.cxx}
#include <TFEL/Math/qt.hxx>
#include <TFEL/Math/st2tost2.hxx>

int main(){
  using namespace tfel::math;
  const auto s = -st2tost2<1u,qt<Stress,double>>::Id();
  return 0;
}
~~~~

The trouble was related to the lack of specialisation of the
`ComputeUnaryOperationResult` metafunction for quantities.

For more details, see <https://github.com/thelfer/tfel/issues/21>.
