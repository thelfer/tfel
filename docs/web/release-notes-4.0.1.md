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

## Issue #132: [mfront] Quantities support in the `StandardElasticity` brick when plane stress support is enabled

For more details, see <https://github.com/thelfer/tfel/issues/132>

## Issue #106: [mfront] Exported metadata about types can be wrong with the new type declaration adopted in `TFEL-4.0`

### Description of the issue

In `TFEL-4.0`, the variable type declaration has been considerably
improved. For example, the following codes are valid:

~~~~{.cxx}
@AuxiliaryStateVariable tvector<2u,real> v;
~~~~

which defines a state variable `v` which is tiny vector in two
dimension, whatever the modelling hypothesis considered. However,
metadata generated by MFront were inconsistent, as `v` was described as
a tiny vector (whose size depends on the dimension).

### Encoding of the type in exported metadata

For most interfaces, `MFront` exports metadata describing the types of
the tensorial variables used by a behaviour, notably for behaviours.

Those metadata can be retrieved by various methods of the
`ExternalLibraryManager` class, such as:
`getGenericBehaviourInitializeFunctionInputsTypes`,
`getGenericBehaviourPostProcessingFunctionOutputsTypes`,
`getUMATGradientsTypes`, `getUMATThermodynamicForcesTypes`, etc.

The types of those variables are encoded as integers.

Those integers shall be intepreted as follows:

- the three first bits, converted in an integer, gives the type of
  variable:
  - `0` denotes a scalar
  - `1` denotes a symmetric tensor
  - `2` denotes a vector
  - `3` denotes an unsymmetric tensor
  - `4` denotes a derivative function
  - `5` denotes an array of objects
- For tensorial objects, the two next bits, converted in an integer,
  gives the space dimension of the variable:
  - `0` indicates that the space dimension depends on the modelling
    hypothesis considered.
  - `1` indicates that the object has a space dimension of `1`,
    indepently of the modelling hypothesis considered.
  - `2` indicates that the object has a space dimension of `2`,
    indepently of the modelling hypothesis considered. 
  - `3` indicates that the object has a space dimension of `3`,
    indepently of the modelling hypothesis considered.
- Concerning a derivative function, the next bits encodes the type of
  function and the bits after the type of the variables with respect to
  which the function is derivated.
- Concerning an array of object, the next three bits encode the arity of
  the array (number of dimensions) and then the sizes of array along
  each dimension, each size being encoding in seven bits. The next bits
  the type of the object hold.

#### Special case of higher order tensors

Higher order tensors are defined as derivative of lower order tensors.

For example, the fourth order tensor of type `t2tost2<N,real>`
(derivative of a symmetric tensor with respect to an unsymmetric tensor)
is associated with identifier `780` as `780 = 4 + (1 << 3) + (3 << 8)`.

#### Special case for tiny vectors

Tiny vectors defined by `tvector<D, value_type>` are encoded as:

- a tensorial object, if `D` is compatible with the size of a tensorial
  object (i.e. `D` has the value `N` or if `D` has an integer value in
  the range `[1;3]`) and if `value_type` is a scalar type.
- an array otherwise.

#### Special case of tiny matrices

Tiny matrices defined by `tmatrix<N, M, value_type>` are encoded as:

- derivative of two tiny vectors, if `N` and `M` can represent the size
  of a tensorial object and if `value_type` is a scalar type.
- an bi-dimensional array otherwise.

For more details, see : <https://github.com/thelfer/tfel/issues/106>.

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
