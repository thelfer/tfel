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

## Ticket #271: [mfront bindings] Support for material properties

The `getDSL` method now returns an object of the `MaterialPropertyDSL`
type which derives from the `AbstractDSL` class. It exposes a method
called `getMaterialPropertyDescription` which returns information about
the material property.

The `MaterialPropertyDescription` exposes the following methods:

- `getInputs` which returns the inputs of the material property.
- `getParameters` which returns the parameters of the material property.
- `getOutput` which returns the output of the material property.
- `getLawName` which returns the name of material property.
- `getMaterialName` which returns the name of material described by this
  material property.
- `getClassName` which returns a class name combining the name of the
  material property and the name of the material.

For more details, see: <https://sourceforge.net/p/tfel/tickets/271/>

## Ticket #269: Allow postprocessing of the inner pressure evolution in PipeTest when imposing the axial radius evolution in the `python` bindings

When the evolution of the outer radius is imposed, the inner pressure evolution can now be retrieved using the following code:
​
~~~~{.python}
p = s.getEvolutionValue("InnerPressure", t)
~~~~
​
where `s` is an object of type `StudyCurrentState` and `t` the considered time.
​
This syntax can also be used to retrieve the "AxialForce" when the axial growth is imposed, as follows:
​
~~~~{.python}
f = s.getEvolutionValue("AxialForce", t)
~~~~

For more details, see: <https://sourceforge.net/p/tfel/tickets/269/>

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

For more details, see: <https://sourceforge.net/p/tfel/tickets/268/>

## Ticket #267: InteractionMatrix coefficients as parameters / material properties {#sec:tfel:3.4.1:issue:267}

For more details, see: <https://sourceforge.net/p/tfel/tickets/267/>
