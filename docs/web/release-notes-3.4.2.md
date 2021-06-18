---
title: Release notes of the 3.4.2 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer
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

The page describes the new functionalities of Version 3.4.2 of the
`TFEL` project.

# New features of `MFront`

## Ability to compute the interaction matrix manually

`MFront` has extensive support for the definition of single crystal as
documented here: <singlecrystal.html>.

If the slip systems are defined (using for example the `@SlipSystem`
keyword) , the class describing the slip system now has a
`buildInteractionMatrix` which is meant to work as follows:

~~~~{.cxx}
@Parameters real im_coefficients[7];
im_coefficients.setEntryName("InteractionMatrixCoefficients");

....

@Integrator{
  const auto& ss = FCCSingleCrystalSlipSystems<real>::getSlipSystems();
  const auto m = ss.buildInteractionMatrix(im_coefficients);
  ....
}
~~~~

This development was made to solve Issue #267 (see Section @sec:tfel:3.4.1:issue:267).

# Issues fixed

## Ticket #268: Missing methods in mtest.MTest for the comparison of the tangent operator to a numerical approximation

The `MTest` executable allows to compare the tangent operator returned
by the behaviour to a numerical approximation. To be able to do the same
using the `python` interface, the following methods have been added:

- `setCompareToNumericalTangentOperator`: set if a comparison of the
  tangent operator returned by the behaviour to a numerical
  approximation.
- `setTangentOperatorComparisonCriterion`: set the criterion used to
  compare the tangent operator returned by the behaviour and its
  numerical approximation.
- `setNumericalTangentOperatorPerturbationValue`: set the perburtation
  of the gradient used to compute a numerical approximation of the
  tangent operator.

## Ticket #267: InteractionMatrix coefficients as parameters / material properties {#sec:tfel:3.4.1:issue:267}

For more details, see: <https://sourceforge.net/p/tfel/tickets/267/>
