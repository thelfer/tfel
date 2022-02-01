---
title: Release notes of the 4.1 version of `TFEL`, `MFront` and `MTest`
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

The page describes the new functionalities of Version 4.1 of the
`TFEL` project.

# `TFEL/System` improvements

## Improvement to the `ExternalLibraryManager` class

### The `hasTemperatureBeenRemovedFromExternalStateVariables` method

In previous versions of `MFront`, the temperature was automatically
defined as the first external state variable by domain specific
languages handling behaviours.

By conventions, for consistency with behaviour interfaces derived from
the `Abaqus`' `UMAT` interface, the temperature was removed from the
list of external state variables exported by the behaviour. This list
can be retrieved using the `getUMATExternalStateVariablesNames` method
of the `ExternalLibraryManager` class.

Following Issue #50 (see Sections
@sec:tfel:mfront:global_options:temperature_as_first_external_state_variable
and @sec:tfel:4.1:issue:50), this automatic declaration is now optional.

For backward compatibility, the `getUMATExternalStateVariablesNames` method
still return the list of external state variables without the
temperature. To know if the temperature was removed, the user must call
the `hasTemperatureBeenRemovedFromExternalStateVariables` method.

In pratice, if the `hasTemperatureBeenRemovedFromExternalStateVariables`
method returns `true`, the full list of external state variables is
given by the temperature followed by the list of external state
variables returned by the `getUMATExternalStateVariablesNames` method.

## Improvements to the `ExternalBehaviourDescription` class

The `ExternalBehaviourDescription` class now have
`hasTemperatureBeenRemovedFromExternalStateVariables` boolean public
data member which states if the temperature was removed from the list of
external state variables.

# `MFront` improvements

## Options to domain specific languages

Domain specific language can have options which can modify their
default behaviour.

### Treat parameters as static variables {#sec:tfel:mfront:global_options:parameters_as_static_variables}

The boolean option `parameters_as_static_variables` modifies the way
parameters are treated. This option can be declared as follows:

~~~~{.cxx}
@DSL Default{parameters_as_static_variables : true};
~~~~

By default, `MFront` behaves as if this option was set to `false`.

If `true`, the parameters will be treated as static variables. In
particular, the values of the parameters can not be changed as runtime.
From the solver point of view, the behaviour does not declare any
parameter.

If the current domain specific language calls other domain specific
languages (for example, a behaviour calling an external material
property), the value of this option is automatically passed to those
domain specific languages (unless superceeded by global options, as
detailled in Section @sec:tfel_4.1:mfront:global_dsl_options).

### Specifying a build identifier {#sec:tfel:mfront:global_options:build_identifier}

In previous versions, a build identifier could be specified using the
`TFEL_BUILD_ID` environment variable.

The `build_identifier` option is another way of specifying the build
identifier, as follows:

~~~~{.cxx}
@DSL IsotropicPlasticMisesFlow{
  build_identifier : "Cyrano-3.4"
};
~~~~

However, the `build_identifier` is not meant to be directly specified in
the `MFront` source file. It shall rather be defined on the command line
(see Section @sec:tfel_4.1:mfront:global_dsl_options).

### Automatic declaration of the temperature as the first external state variable for behaviours {#sec:tfel:mfront:global_options:temperature_as_first_external_state_variable}

In previous versions of `MFront`, the temperature was automatically
defined as the first external state variable by domain specific
languages handling behaviours, as this is required by most behaviour
interfaces derived from `Abaqus`' `UMAT` interface.

This automatic declaration can now be disabled using the
`automatically_declare_temperature_as_first_external_state_variable`
boolean option, as follows:

~~~~{.cxx}
@DSL IsotropicPlasticMisesFlow{
  automatically_declare_temperature_as_first_external_state_variable : false
};
~~~~

By default, `MFront` behaves as if this option was set to `true`.
Currently, only the generic interface supports behaviours which do not
declare the temperature a the first external state variable.

#### Disable initialization of parameters from text file {#sec:tfel:mfront:global_options:initialize_parameters_from_file}

By default, parameters may be initialized from a text file in the
current directory, if this text file exists. This feature is implemented
and enabled by default for behaviours. For material properties, this
feature is implemented by some interfaces (`Cast3M`, `Cyrano`, `Octave`,
`Python`).

This behaviour can now be changed by using the
`initialize_parameters_from_file` boolean option.

#### Special case of specialized domain specific languages

`IsotropicMisesCreep`, `IsotropicMisesPlasticFlow`,
`IsotropicStrainHardeningMisesCreep` and `MultipleIsotropicMisesFlows`
domain specific languages used to automatically declare the temperature
at the middle of the time step in a local variable named `T_`. This
declaration is disabled if the
`automatically_declare_temperature_as_first_external_state_variable`
option is set to `false`.

### Defining global options from the command line {#sec:tfel_4.1:mfront:global_dsl_options}

Options passed to domain specific languages can be defined globally
using one of the following command line arguments:

- `--dsl-option`, which allows to define an option for all domain
  specific languages.
- `--material-property-dsl-option`, which allows to define an option for
  all domain specific languages related to material properties.
- `--behaviour-dsl-option`, which allows to define an option for all
  domain specific languages related to behaviours.
- `--model-dsl-option`, which allows to define an option for all domain
  specific languages related to models.

The options defined by command line arguments are merged with the
options defined inside the `MFront` file. In case of conflicts, an
option defined on the command-line overwrites the option defined in the
`MFront` file.

#### Example of usage

~~~~{.cxx}
$ mfront --obuild --interface=generic                          \
    --behaviour-dsl-option=parameters_as_static_variables:true \
    Plasticity.mfront
~~~~

### Retrieving the list of options associated with a domain specific language {#sec:tfel_4.1:mfront:list_dsl_options}

The list of options associated with a domain specific language can be
retrieved using the `--list-dsl-options` command line argument as
follows:

~~~~{.bash}
$ mfront --list-dsl-options=RungeKutta
- parameters_as_static_variables: boolean stating if the parameter shall be treated as static variables.
- automatically_declare_temperature_as_first_external_state_variable: boolean stating if the temperature shall be automatically declared as an external state variable.
~~~~

## Post-processing of behaviours


# `MTest` improvements

## Support for a boundary condition modelling the effet of a mandrel in pipe modelling {#sec:tfel_4.1:mtest:mandrel}

The effect of a non-deformable mandrel located inside the pipe can be
modelled by defining the evolution of its radius \(R_{m}\) using the
`@MandrelRadiusEvolution` keyword. The inner radius \(R_{i}\) will then
satisfy the following unilateral boundary condition:

\[
R_{i} - R_{m} \geq 0
\]

This boundary condition is imposed by using a Lagrange multiplier. Its
value is given by the contact pressure in the output file. The total
pressure applied to the inner surface of the pipe is given by the sum of
the imposed pressure (if any) and the contact pressure. Only the imposed
inner pressure is used to compute the end cap effect.

This boundary condition is not compatible with:

- boundary conditions imposing the evolution of the inner radius of the
  pipe or the outer radius of the pipe.
- the modelling of a tight pipe. 

### Axial binding

If the evolution of the axial growth of the mandrel is defined using the
`@MandrelAxialGrowthEvolution` keyword, an axial binding between the
mandrel and the pipe is assumed, i.e. the difference between the axial
growth of the pipe \(\varepsilon^{p}_{zz}\) and the axial growth of the
mandrel \(\varepsilon^{p}_{zz}\) is assumed to be constant to its value
when the contact between the mandrel and the pipe is detected:

\[
\varepsilon^{p}_{zz}-\varepsilon^{m}_{zz}=\textrm{Cste}
\]

This axial boundary condition is not compatible with the boundary
condition imposing the evolution of the axial growth of the pipe.

## Wrappers around `3D` behaviours {#sec:tfel_4.1:mtest:small_strain_tridimensional_behaviour_wrapper}

The `SmallStrainTridimensionalBehaviourWrapper` class allows to wrap a
tridimensional behaviour into a behaviour usable in one of the following
modelling hypotheses:

- axisymmetrical generalised plane strain,
- generalised plane strain,
- plane strain.

Shear components of the strain tensor which are not meaningful for the
targeted modelling hypothesis are set to zero.

After the behaviour integration, only the meaningful components of the
stress tensor and stiffness matrix are retained.

### Treatment of the internal state variables

The internal state variables are all declared as scalars.

For instance, let us assume that the `3D` behaviour declares an internal
state variable called `s`. In this case, the wrapped behaviour will then
declared `6` internal state variables associated with `s` named
respectively `sXX`, `sYY`, `sZZ`, `sXY`, `sXZ`, `sYZ`.

### Usage in `MTest`

The following code shows how to wrap a tridimensional behaviour to be
used under the axisymmetrical plane strain modelling hypothesis:

~~~~{.cxx}
// Choice of the modelling hypothesis
@ModellingHypothesis "AxisymmetricalGeneralisedPlaneStrain";
// Declaration of the behaviour
@Behaviour<
    generic,                                   // interface name
    SmallStrainTridimensionalBehaviourWrapper> // wrapper
    "src/libBehaviour.so" "ImplicitNorton";
~~~~

### Usage in the `mtest` `python` module

The following code shows how to load a tridimensional behaviour and how
to wrap this behaviour to be used in plane strain:

~~~~{.python}
import mtest
import tfel.material
h = tfel.material.ModellingHypothesis.TRIDIMENSIONAL
b1 = mtest.Behaviour('generic', 'src/libBehaviour.so',
                     'ImplicitNorton', h)
# ['ElasticStrain', 'p']
print(b1.getInternalStateVariablesNames())
b2 = mtest.Behaviour(wrapper = 'SmallStrainTridimensionalBehaviourWrapper',
                     library = 'src/libBehaviour.so',
                     function = 'ImplicitNorton',
                     hypothesis = 'PlaneStrain')
# ['ElasticStrainXX', 'ElasticStrainYY', 'ElasticStrainZZ',
#  'ElasticStrainXY', 'ElasticStrainXZ', 'ElasticStrainYZ', 'p']
print(b2.getInternalStateVariablesNames())
~~~~

## A more flexible declaration of the rotation matrix {#sec:tfel_4.1:mtest:rotation_matrix}

The `@RotationMatrix` keyword now has a `Direction` option which now let
the user specify:

- a \(2D\) vector giving the first direction of orthotropy in modelling
  hypotheses of space dimension \(2\).
- two \(3D\) vectors giving respectively the first and second directions
  of orthotropy under the tridimensional modelling hypothesis.

The given vectors are not required to be normalised. In the \(3D\) case,
the second vector is not required to be orthogonal to the first one. If
not, the second direction of orthotropy is deduced from the second
vector by removing its projection along the first one.

### Usage

~~~~{.cpp}
// using direction in 2D
@RotationMatrix<Direction> {0,1};
~~~~

~~~~{.cpp}
// using directions in 3D
@RotationMatrix<Direction> {{0,1,0},{1,0,0}};
~~~~

## Wrapper around behaviours written in the logarithmic strain framework generated with the `aster` interface {#sec:tfel_4.1:mtest:aster_logarithmic_strain_wrapper}

Behaviours written in the logarithmic strain framework and generated
with the `aster` interface were not handled by `MTest` because the
logarithmic strain framework is treated by `code_aster` and not by the
`aster` interface.

However, this is an issue for users using `MTest` for the identification
of the behaviour, as described in [Issue
#55](https://github.com/thelfer/tfel/issues/55).

`MTest` now automatically wraps the behaviour to handle those
behaviours. The wrapper handles the pre and post-processing steps around
the behaviour integration.

# Improvements to the `mtest` `python` module

## Support of named arguments in the constructor of the `Behaviour` class {#sec:tfel_4.1:pymtest:behaviour_constructor}

Named arguments are now supported in the `Behaviour` constructor. The
following arguments can be specified:

- `interface`: name of the interface to be used (optional).
- `library`: name of the shared library to be loaded.
- `function`: name of the function implementing the behaviour.
- `hypothesis`: hypothesis to be used.
- `wrapper`: name of the behaviour wrapper to be used (optional).

### Example of usage

The following piece of code loads a behaviour implemented in the
`src/libBehaviour.so` shared library by the `ImplicitNorton` function
for the `PlaneStrain` modelling hypothesis:

~~~~{.python}
b = mtest.Behaviour(library = 'src/libBehaviour.so',
                    function = 'ImplicitNorton',
                    hypothesis = 'PlaneStrain')
~~~~

# `mfront-query` improvements

## List of post-processings

### Example of usage

~~~~{.bash}
$ mfront-query --post-processings Elasticity.mfront 
- PrincipalStrain: compute the principal strain. Modified post-processing variables are:
  - PrincipalStrain (εᵖ)
~~~~

## List of post-processing variables

~~~~{.bash}
$ mfront-query --post-processing-variables Elasticity.mfront 
- PrincipalStrain (εᵖ)
~~~~

# Issues fixed

## Issue 94: [mfront] Add option to disable initialization of parameters from text file

This feature is described in Section
@sec:tfel:mfront:global_options:initialize_parameters_from_file.

For more details, see : <https://github.com/thelfer/tfel/issues/94>.

## Issue 91: [mfront] define build identifier using options

This feature is described in Section
@sec:tfel:mfront:global_options:build_identifier.

For more details, see : <https://github.com/thelfer/tfel/issues/91>.

## Issue #83: [mfront] Add a command line argument to retrieve the list of options associated with a domain specific language

This feature is described in Section
@sec:tfel_4.1:mfront:list_dsl_options.

For more details, see : <https://github.com/thelfer/tfel/issues/83>.

## Issue #82: [mfront] Ability to define DSL options on the command line

This feature is described in Section
@sec:tfel_4.1:mfront:global_dsl_options.

For more details, see : <https://github.com/thelfer/tfel/issues/82>.

## Issue #55: [mtest] Creation of a wrapper around behaviours written in the logarithmic strain framework generated with the `aster` interface

The wrapper is described in Section
@sec:tfel_4.1:mtest:aster_logarithmic_strain_wrapper.

For more details, see : <https://github.com/thelfer/tfel/issues/55>.

## Issue #57: [gallery] Mistakes in the Drucker Prager Cap model of the gallery

For more details, see : <https://github.com/thelfer/tfel/issues/57>.

## Issue #50: [mfront] Options to avoid the automatic declaration of the temperature as an external state variable {#sec:tfel:4.1:issue:50}

This option is described in depth in Section
@sec:tfel:mfront:global_options:temperature_as_first_external_state_variable.

For more details, see : <https://github.com/thelfer/tfel/issues/50>.

## Issue #44: [mtest] Add support for a boundary condition modelling the effet of a mandrel in pipe modelling

The feature is described in Section @sec:tfel_4.1:mtest:mandrel.

For more details, see : <https://github.com/thelfer/tfel/issues/38>.

## Issue #39: [tfel-config] Add query associated with python bindings support

`tfel-config` now supports an command line option named
`--python-bindings-support` which displays true if `python` bindings are
available, `false` otherwise.

For more details, see : <https://github.com/thelfer/tfel/issues/38>.

## Issue #38: [tfel-config] Add query associated with MADNEX support

`tfel-config` now supports an command line option named
`--madnex-support` which displays true if `madnex` files are supported,
`false` otherwise.

For more details, see : <https://github.com/thelfer/tfel/issues/38>.

## Issue #36: [mfront] Improved support for `MTest` file generation (all kind of state variables)

In previous versions, only scalar and symmetric tensors state variables
were supported when generation `MTest` file on integration failure.

All kind of state variables are now supported.

For more details, see : <https://github.com/thelfer/tfel/issues/36>.

## Issue #28: [mtest] Support of tensorial external state variables

The `Behaviour` class has new methods:

- `expandExternalStateVariablesNames` (wrapped in python)
- `getExternalStateVariableType`  (wrapped in python)
- `getExternalStateVariablePosition` (wrapped in python)

The `@ExternalStateVariable` now allows to define tensorial external
state variables components by components or an array as follows:

~~~~{.cpp}
// Definition of an symmetric tensor external state variable
@ExternalStateVariable<function> "g" {"0", "1", "2", "3 * t", "4", "5 * t"};
// Definition of an symmetric tensor external state variable components by components
@ExternalStateVariable<function> "g2XX"  "0";
@ExternalStateVariable<function> "g2YY"  "2";
@ExternalStateVariable<function> "g2ZZ"  "t";
@ExternalStateVariable<function> "g2XY"  "3";
@ExternalStateVariable<function> "g2XZ"  "4";
@ExternalStateVariable<function> "g2YZ"  "5*t";
~~~~

For more details, see : <https://github.com/thelfer/tfel/issues/28>.

## Issue #28: [mtest] Support of tensorial external state variables

For more details, see : <https://github.com/thelfer/tfel/issues/28>.

## Issue #27: [mfront] Better support of tensorial external state variables

The type of the external state variables are now exported.

- `ExternalLibraryManager` has a new method called
  `getUMATExternalStateVariablesTypes` (wrapped in `python`)
- `ExternalBehaviourDescription` has a new method called `evtypes`
  (wrapped in `python`)

Generation of `MTest` file also supports tensor external state variables.

For more details, see : <https://github.com/thelfer/tfel/issues/27>.

## Issue #26: Don't use explicit instanciation for static variables

For more details, see : <https://github.com/thelfer/tfel/issues/26>.

## Issue #23: [mfront] Remove usage of C++ reserved variables names

Some exported variables used to contain a double underscore in their
names. Such names are reserved by the C++ standard for internal use by
the compilers.

For more details, see : <https://github.com/thelfer/tfel/issues/23>.

## Issue #22 : [mfront] Option to treat parameters as static variables

This option is described in depth in Section
@sec:tfel:mfront:global_options:parameters_as_static_variables.

For more details, see : <https://github.com/thelfer/tfel/issues/22>.

## Issue #20: [mtest] Declaration of a behaviour wrapper in python module

As described in Section @sec:tfel_4.1:pymtest:behaviour_constructor,
named parameters are now supported in the constructor of the `Behaviour`
class. The (optional) `wrapper` argument can now be used to specify a
behaviour wrapper.

Currently, two behaviours wrappers are available:

- `LogarithmicStrain1D`: which turns a small strain behaviour in
  axisymmetrical generalised plane strain into a finite strain behaviour
  in the logarithmic strain framework following the ETO-PK1 kinematic
  assumption used by the `MTest` for the simulation of pipe in finite
  strain.
- `SmallStrainTridimensionalBehaviourWrapper` which allows to use a
  small strain tridimensional behaviour under the following modelling
  hypotheses: axisymmetrical generalised plane strain, plane strain,
  axisymmetry.

For more details, see : <https://github.com/thelfer/tfel/issues/20>.

## Issue #18: [mtest] Usage of a `3D` behaviour in `ptest`

Thanks the `SmallStrainTridimensionalBehaviourWrapper` class detailled
in Section
@sec:tfel_4.1:mtest:small_strain_tridimensional_behaviour_wrapper,
tridimensional behaviours can be used under the following modelling
hypotheses: generalised plane strain, plane strain, axisymmetrical
generalised plane strain.

For more details, see : <https://github.com/thelfer/tfel/issues/18>.

## Issue #15: [mtest] New option for the declaration of the rotation matrix in `MTest`

This option is described in depth in Section
@sec:tfel_4.1:mtest:rotation_matrix.

For more details, see : <https://github.com/thelfer/tfel/issues/15>.

<!--
# References
-->