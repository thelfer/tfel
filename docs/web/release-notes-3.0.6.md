---
title: Release notes of the 3.0.6 version of `TFEL`, `MFront` and `MTest`
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
tabPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

# Noticeable fixed issues that may affect the results obtained with previous versions

Ticket #256 reported that the scalar product of two unsymmetric tensors
was not properly computed.

This may affect single crystal finite strain computations to a limited
extent, as the Mandel stress tensor is almost symmetric.

# Tickets fixed

## Ticket #256: Scalar products of unsymmetric tensors

The scalar product of unsymmetric tensors was not computed correctly in
\(2D\) and \(3D\).

For more details, see: <https://sourceforge.net/p/tfel/tickets/256/>


## Ticket #254: Register libraries for `ldconfig`

In previous versions, the build version and the API version of shared
libraries were not specified.

For more details, see: <https://sourceforge.net/p/tfel/tickets/254/>

## Ticket #246: Explicit implementation of the Berveiller-Zaoui homogeneisation scheme is inefficient

Several polycrystal behaviours based on the Berveiller-Zaoui
homogeneisation scheme are available in the tests of `MFront`.

For some unknown reasons, those implementations declared the equivalent
plastic strain as an auxiliary state variable. This lead to a
significantly more difficult convergence of the `Runge-Kutta` algorithm.

Since the results are correct, those implementations are kept as they
are, but new corrected implementations are provided. Appropriate
warnings are added to the description of flawed behaviours. Users are
advised to use the newly proposed implementation.

For more details, see: <https://sourceforge.net/p/tfel/tickets/246/>

## Ticket #244: Some header files related to MTest are not installed by `cmake`

The following headers were not installed by `cmake`:

- `SolverWorkSpace.hxx`
- `PipeTest.hxx`
- `PipeMesh.hxx`
- `PipeProfileHandler.hxx`

For more details, see: <https://sourceforge.net/p/tfel/tickets/244/>

## Ticket #239: Missing `ostream` header for `gcc-10.1.0`

For more details, see: <https://sourceforge.net/p/tfel/tickets/239/>

## Ticket #238: `getPartialJacobianInvert` is broken for arrays of internal state variables

For more details, see: <https://sourceforge.net/p/tfel/tickets/238/>

## Ticket #224: Fix compilation of the `gentype3.cxx` test in `C++-17` with `gcc-9.2`

For more details, see: <https://sourceforge.net/p/tfel/tickets/208/>

## Ticket #208: Support of the `@GasEquationOfState` keyword is broken in `MTest`

`MTest` is able to model a tight pipe. This hypothesis assumes that the
amount of substance of the gas inside the pipe is constant. The
evolution of the inner pressure is linked to the temperature and radius
evolution through the gas equation of state. By default, the equation of
state of a perfect gas is used.

The `@GasEquationOfState` keyword was introduced to specify the equation
of state of the gas. This feature has never been tested and was not
functional in previous versions.

For more details, see: <https://sourceforge.net/p/tfel/tickets/208/>

## Ticket #207: Circumvent a possible bug in `FindBoost` for `cmake-3.16`

Two boost components are searched when enabling `python` bindings:
`boost-python` and `boost-numpy`. The first one is required and the
second one is optional.

The detection process that was used in previous versions called
`find_package` for two components at the same time and then we checkd of
two components were found.

In `cmake` 3.16 (at least), the behaviour of `find_package` changed and
if one component is missing, both are declared not found.

We now check for `boost-python` first and then search for `boost-numpy`.

For more details, see: <https://sourceforge.net/p/tfel/tickets/207/>

## Ticket #197: Allow using flags defined in standard environment variables to build `TFEL/MFront`

The variable `USE_EXTERNAL_COMPILER_FLAGS` can now be defined in `cmake`
to use the flags defined in the environment variables `CFLAGS` and
`CXXFLAGS`.

For more details, see: <https://sourceforge.net/p/tfel/tickets/197/>
