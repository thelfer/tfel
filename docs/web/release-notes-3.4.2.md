---
title: Release notes of the 3.4.2 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer
date: 21/09/2021
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

This version was released This version was released on September 21,
2021 along with Versions `3.0.9`, `3.1.9`, `3.2.6` an `3.3.2` and
inherits from the issues solved in those versions.

# New features of `MFront`

## Ability to compute the interaction matrix manually {#sec:tfel:3.4.2:mfront:interaction_matrix}

This development was made to solve Issue #267 (see Section @sec:tfel:3.4.2:issue:267).

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

# New features of `MTest`

## Basic support for material properties {#sec:tfel:3.4.2:mtest:material_properties}

This development was made to solve Issue #273 (see Section
@sec:tfel:3.4.2:issue:273).

An abstract class named `MaterialProperty` has been added as an
equivalent for material properties of the `Behaviour` class. This class
has the following interface:

- `getVariablesNames`, which returns the names of the arguments of
  material property.
- `setVariableValue` which allows to set the value of an argument of the
  material property.
- `getValue` which evaluates the material property for the values of the
  arguments set by the `setVariableValue` method.
- `getParametersNames`, which returns the names of the parameters of
  material property.
- `setParameter` which allows to change the value of a parameter.

The `CastemMaterialProperty` and `CyranoMaterialProperty` classes are
two concrete implementation of the class for material properties
generated respectively using the `Cast3M` and `Cyrano` interfaces
respectively.

In `C++`, the `MaterialProperty` class exposes a static method called
`getMaterialProperty` which acts as an abstract factory.

This class and associated method are also available in the `python`
bindings. The `getMaterialProperty` method is used in `python` to build
a custom constructor as illustrated in the following example. 

### Example

Here is an example of the usage of the `MaterialProperty` class in `python`.

~~~~{.python}
import mtest
young_modulus = mtest.MaterialProperty(
    'src/libCastemVanadium.so', 'VanadiumAlloy_YoungModulus_SRMA')
young_modulus.setVariableValue('Temperature', 562)
E = young_modulus.getValue()
~~~~

Note that the `MaterialProperty` constructor automatically detects the
interface used to generate the material property and instantiates the
proper concrete implementation internally which is obsviously
the `CastemMaterialProperty` class in this case.

Setting the variables' values and evaluating the material property can
be tedious. To avoid this, overloaded versions of the `getValue` are
available:

~~~~{.python}
# using a dictionary
E = young_modulus.getValue({'Temperature': 562})
# for material properties with only one argument
E = young_modulus.getValue(562)
~~~~

To make the code even more explicit, the call operator can also be used,
as follows:

~~~~{.python}
# using a dictionary
E = young_modulus({'Temperature': 562})
# for material properties with only one argument
E = young_modulus(562)
~~~~

# Issues fixed

## Issue #8: [python bindings] Implementation error in overrideModelName in OverridableImplementation

For more details, see: <https://github.com/thelfer/tfel/issues/8>

## Issue #274:  [material properties] Allow to specify types of the output, inputs and parameters

The output, inputs and parameters of a material property can now be
declared with any scalar type supported by the `SupportedTypes` class.

For more details, see: <https://sourceforge.net/p/tfel/tickets/274/>

## Issue #273: [mtest] Add basic support for testing material properties {#sec:tfel:3.4.2:issue:273}

The developments associated with this issue are described in Section
@sec:tfel:3.4.2:mtest:material_properties.

For more details, see: <https://sourceforge.net/p/tfel/tickets/273/>

## Issue #272: [material properties] Export parameters list, bounds and default values

The list of parameters is now exported by interfaces of material
properties which provides a `setParameter` method (namely the `Cast3M`
and `Cyrano` interfaces).

The `ExternalLibraryManager` now have an `getMaterialPropertyParameters`
method which allows to retrieve this list of parameters. This method is
also available in the `python` bindings.

The `ExternalMaterialPropertyDescription` class which can gathers all
the informations exported by a material property new have a `parameters`
data member with the list of the parameters of the material property.
This data member is also accessible in the `python` bindings.

For more details, see: <https://sourceforge.net/p/tfel/tickets/272/>

## Issue #271: [mfront bindings] Support for material properties

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

## Issue #269: Allow postprocessing of the inner pressure evolution in PipeTest when imposing the axial radius evolution in the `python` bindings

When the evolution of the outer radius is imposed, the inner pressure
evolution can now be retrieved using the following code:

~~~~{.python}
p = s.getEvolutionValue("InnerPressure", t)
~~~~

where `s` is an object of type `StudyCurrentState` and `t` the
considered time.

This syntax can also be used to retrieve the "AxialForce" when the axial
growth is imposed, as follows:

~~~~{.python}
f = s.getEvolutionValue("AxialForce", t)
~~~~

For more details, see: <https://sourceforge.net/p/tfel/tickets/269/>

## Issue #268: Missing methods in mtest.MTest for the comparison of the tangent operator to a numerical approximation

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

## Issue #267: InteractionMatrix coefficients as parameters / material properties {#sec:tfel:3.4.2:issue:267}

The developments associated with this issue are described in Section
@sec:tfel:3.4.2:mfront:interaction_matrix.

For more details, see : <https://sourceforge.net/p/tfel/tickets/267/>
