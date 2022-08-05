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

## Issue 274: [mfront] Analysers not called while parsing `@Integrator` and `@Predictor` in implicit DSLs

For more details, see <https://github.com/thelfer/tfel/issues/274>.

## Issue #251: [mtest] Bad definition of `PipeMeshElementType` in `python` bindings

For more details, see <https://github.com/thelfer/tfel/issues/251>.

## Issue #162: [mfront] Minors issues of syntax in `StandardElastoViscoPlasticity` Brick

For more details, see <https://github.com/thelfer/tfel/issues/162>.

## Issue #159: Failing to generate input examples shall not stop mfront

For more details, see <https://github.com/thelfer/tfel/issues/159>.

## Issue #140: [mtest] lonely separator is not correctly treated

For more details, see <https://github.com/thelfer/tfel/issues/140>.

## Issue #43: [mtest] Prevent declaration of boundary conditions after the call to completeInitialisation

For more details, see <https://github.com/thelfer/tfel/issues/43>.

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
