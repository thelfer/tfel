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

# `MTest` improvements

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

# Issues fixed

## Issue #20: Declaration of a behaviour wrapper in python module

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

For more details, see : <https://github.com/thelfer/tfel/issues/20>

## Issue #18: Usage of a `3D` behaviour in `ptest`

Thanks the `SmallStrainTridimensionalBehaviourWrapper` class detailled
in Section
@sec:tfel_4.1:mtest:small_strain_tridimensional_behaviour_wrapper,
tridimensional behaviours can be used under the following modelling
hypotheses: generalised plane strain, plane strain, axisymmetrical
generalised plane strain.

For more details, see : <https://github.com/thelfer/tfel/issues/18>

<!--
# References
-->