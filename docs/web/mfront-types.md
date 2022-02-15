---
title: Predefined aliases in `MFront`
author: Thomas Helfer
date: 17/08/2021
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

This page described:

- How the type of a variable is analysed by `MFront`.
- The type aliases automatically defined in `MFront`.

# Variable declaration

The declaration of variables in `MFront` has the following syntax:

~~~~{.cxx}
keyword type variable1, variable2, ...; 
~~~~

where:

- `keyword` allows to specify the kind of variables declared. The
  keywords associated with type declaration may depend on the domain
  specific language used. For example, most DSLs associated with
  behaviours, allow the `@MaterialProperty`, `@Parameter`,
  `@LocalVariable`, `@StateVariable`, `@AuxiliaryStateVariable`
  keywords to introduce a new variable.
- `type` declares the type of the (`C++`) variable. Any `C++` type may
  be used, although there may be limitations specifics to the kind of
  variables being treated or the interfaces used:
  - The type of a local variable barely only has the constraint that
    variables of this type must be default constructible.
  - Most interfaces require material properties and external state
    variables to be scalars.
  - State variables must be scalars or tensors
  - etc.
- The name of the variables (`variable1`, `variable2`, etc...) are
  restricted to be valid `C++` variables names, although unicode symbols
  may be used as described on [this page](unicode.html).

Concerning the type of a variable, `MFront` also introduces a few type
alias, described in Section @sec:mfront_types:type_aliases, and some
shorcuts which are now detailled.

## Integers variables in template arguments

Integer template arguments can be defined using an integer or by a
formula that may contains two kind of integer variables: integer
constants and `constexpr` integer variables.

### Integer constants

All integer constants, introduced by the `@IntegerConstant` keyword, can
be used as template arguments.

~~~~{.cxx}
@IntegerConstant Nss = 12;
...
@LocalVariable fsarray<Nss, real> τᶜ;
~~~~

### `constexpr` integers variables defined by behaviours

By default, `MFront` DSLs associated with behaviours automatically
defines the following `constexpr` integers variables:

- `N`: the space dimension
- `TVectorSize`: the natural size of a tiny vector, equal to the space
  dimension.
- `StensorSize`: the natural size of a symmetric tensor for the space
  dimension `N`, i.e.:
  - 3 in `1D`.
  - 4 in `2D`.
  - 6 in `3D`.
- `TensorSize`: the natural size of a tensor for the space dimension
  `N`, i.e.:
  - 3 in `1D`.
  - 5 in `2D`.
  - 9 in `3D`.

## Tensorial objects automatically imported from `TFEL/Math`

The following types are automatically imported from `TFEL/Math`:

- `fsarray`: a generic fixed size container.
- `tvector`: a class which represents fixed size vectors.
- `tmatrix`: a class which represents fixed size matrices.
- `stensor`: a class which represents symmetric tensors.
- `tensor`: a class which represents unsymmetric tensors.
- `st2tost2`: a class which represents linear operations between
  symmetric tensors.
- `t2tot2`: a class which represents linear operations between
  unsymmetric tensors.
- `t2tost2`: a class which represents linear operations transforming an
  unsymmetric tensor in a symmetric tensor.
- `st2tot2`: a class which represents linear operations transforming a
  symmetric tensor in an unsymmetric tensor.

Those types have template parameters which specifies their size and the
numeric type used.

The latter can be a quantity as discussed in the next paragraph.

The following declaration:

~~~~{.cxx}
@StateVariable tfel::math::stensor<N, strain> evp;
~~~~

is thus equivalent to:

~~~~{.cxx}
@StateVariable stensor<N, strain> evp;
~~~~

i.e. the `tfel::math` namespace can be obmitted.

As described in Section @sec:mfront_types:type_aliases, this declaration
is also equivalent to:

~~~~{.cxx}
@StateVariable StrainStensor evp;
~~~~

## The `quantity` type

Quantities are scalars with units. They are supported if the `@UseQt`
keyword has been used with the `true` argument.

They are represented by the `quantity` class which has up to eight
template parameters.

The first template argument shall designate a floatting-point number
type. As discussed in Section @sec:mfront_types:type_aliases, `real` is
in reality a type alias to the floatting point actually used by the
calling solver and which is selected by the solver' interface.

The next template arguments are integers which defines the unit
associated with the quantity, following the internal system of units. In
this system, a unit is decomposed as follows:

\[
kg^{i_{1}}\,m^{i_{2}}\,s^{i_{3}}\,A^{i_{4}}\,K^{i_{5}}\,cd^{i_{6}}\,mol^{i_{7}}
\]

In `MFront`, units are thus defined by the seven integers \(i_{1},
\ldots \,i_{7}\). If one of this integer is not specified in the
definition of a quantity, it is defaulted to \(0\).

The following declarations are thus equivalent:

- `@LocalVariable length l;` (see Section @sec:mfront_types:type_aliases)
- `@LocalVariable quantity<real,0,1> l;`

Note that many aliases, described in Section
@sec:mfront_types:type_aliases, are provided by `MFront` to avoid direct
usage of the `quantity` type.

## The `derivative_type` alias

`derivative_type` is an alias to the type of derivative a
mathematical object with respect to another. For example, the derivative
of a symmetric tensor with respect to a tensor can be declared as
follows:

~~~~{.cxx}
@LocalVariable derivative_type<stensor<N,stress>,tensor<N,real>> K;
~~~~

See Section @sec:mfront_types:type_aliases for the meaning of the stress
type.

This declaration is equivalent to:

~~~~{.cxx}
@LocalVariable t2tost2<N, stress> K;
~~~~

## The `inverse_type` alias

`inverse_type` is an alias the type of the inverse of its template
argument. For example:

~~~~{.cxx}
@LocalVariable inverse_type<time> f;
~~~~

## The `result_type` alias

`result_type` is an alias to the result of a binary operation. 

~~~~{.cxx}
@LocalVariable result_type<strain,time,OpDiv> de0;
~~~~

The available operations are `OpPlus`, `OpMinus`, `OpMult`, and `OpDiv`.

# Type aliases {#sec:mfront_types:type_aliases}

If quantities are not used, all scalar types are a simple alias to the
numeric type used. This statement can obviously be generalised to the
other mathematical objects.

## Scalar types

- `real`: the numeric type used.
- `time`: a scalar with the dimension of a time.
- `frequency`: a scalar with the dimension of a frequency.
- `length`: a scalar with the dimension of a lenght.
- `inv_length`: a scalar with the dimension of the inverse of a length.
- `displacement`: a scalar with the dimension of a lenght.
- `strain`: a scalar without dimension.
- `strainrate`:  a scalar with the dimension of a frequency.
- `force`: a scalar with the dimension of a force.
- `stress`: a scalar with the dimension of a stress.
- `stressrate`: a scalar with the dimension of a stress divided by a
  time.
- `temperature`:  a scalar with the dimension of a temperature.
- `thermalexpansion`: a scalar representing a thermal expansion
  coefficient with the dimension of the inverse of a temperature.
- `massdensity`: a scalar with the dimension of a mass divided by a
  volume.
- `energydensity`: a scalar with the dimension of an energy divided by a
  volume.
- `speed`: a scalar with the dimension of an length divided by a
  time.
- `thermalconductivity`: a scalar with the dimension of power divided by
  a length, a time and a temperature. In the International System of
  Units, the thermalconductiviy is measured in watts per meter-kelvin
  (\(W/(m\,K)\)).

## Vector types

- `TVector`: 
- `ForceTVector`: 
- `DisplacementTVector`:
- `HeatFlux`:

## Matrix types

- `ThermalConductivityMatrix`:

## Symmetric tensor types

- `Stensor`: 
- `FrequencyStensor`: 
- `StressStensor`: 
- `StressRateStensor`: 
- `StrainStensor`: 
- `StrainRateStensor`: 
- `ThermalExpansionCoefficientTensor`: 

## Tensor types

- `Tensor`: 
- `FrequencyTensor`: 
- `StressTensor`: 
- `DeformationGradientTensor`: 
- `DeformationGradientRateTensor`: 

## Fourth order tensor types of the `st2tost2` type

- `Stensor4`: fourth order tensor type for the current modelling
  hypothesis holding the current numeric type (no unit).
- `StiffnessTensor`: fourth order tensor type for the current modelling
  hypothesis holding stress values.

# Encoding types of variables in exported metadata

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

## Example

- `0` denotes a scalar
- `17` denotes a two-dimensional symmetric tensor 

## Special case of higher order tensors

Higher order tensors are defined as derivative of lower order tensors.

For example, the fourth order tensor of type `t2tost2<N,real>`
(derivative of a symmetric tensor with respect to an unsymmetric tensor)
is associated with identifier `780` as `780 = 4 + (1 << 3) + (3 << 8)`.

## Special case for tiny vectors

Tiny vectors defined by `tvector<D, value_type>` are encoded as:

- a tensorial object, if `D` is compatible with the size of a tensorial
  object (i.e. `D` has the value `N` or if `D` has an integer value in
  the range `[1;3]`) and if `value_type` is a scalar type.
- an array otherwise.

## Special case of tiny matrices

Tiny matrices defined by `tmatrix<N, M, value_type>` are encoded as:

- derivative of two tiny vectors, if `N` and `M` can represent the size
  of a tensorial object and if `value_type` is a scalar type.
- an bi-dimensional array otherwise.
