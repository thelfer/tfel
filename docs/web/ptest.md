---
title: "Simulation of pipes in MTest"
author: Thomas Helfer
date: 08/02/2026
lang: en-EN
link-citations: true
colorlinks: true
geometry:
  - margin=2cm
papersize: a4
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

# Usage

`MTest` can also be used to model a pipe subjected to various loadings.
This extension to pipes is often referred to as `PTest`.

By default, `MTest` considers a unique material point. Switching to pipe
modeling can be done by either using a file with the extension `.ptest`
or by using the command-line option `--scheme=ptest`.

## Getting help on the command line

The following line allows displaying the list of supported keywords:

~~~{.bash}
mtest --scheme=ptest --help-keywords-list
~~~

The help associated with a given keyword can be displayed using the
following `--help-keyword` command line argument, as in the following
example:

~~~{.bash}
mtest --scheme=ptest --help-keyword=@RadialLoading
~~~~

## A simple tutorial



# Overview and modeling hypotheses


# Behaviours

<!--
## Issue 20: [mtest] Declaration of a behaviour wrapper in python module

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

## Issue 18: [mtest] Usage of a `3D` behaviour in `ptest`

Thanks the `SmallStrainTridimensionalBehaviourWrapper` class detailled
in Section
@sec:tfel_4.1:mtest:small_strain_tridimensional_behaviour_wrapper,
tridimensional behaviours can be used under the following modelling
hypotheses: generalised plane strain, plane strain, axisymmetrical
generalised plane strain

-->

# Geometry and finite element discretization

## Geometry

The geometry of the pipe is specified using the self-explanatory
keywords `@InnerRadius` and `@OuterRadius`.

## Finite element discretization

The finite element discretization is specified using the
self-explanatory keywords `@ElementType` and `@NumberOfElements`. The
following element types are supported: `Linear` and `Quadratic`. By
default, quadratic elements are used.

## Type of analysis

By default, simulations are done in a finite strain framework. The
`@PerformSmallStrainAnalysis`, which is followed by a boolean, allows
switching to a resolution under the infinitesimal strain hypothesis.

# Radial loadings

The `RadialLoading` keyword let the user choose the kind of loading to
be applied to the pipe in radial direction.

This keyword is followed by a string. The following loading are supported:

- `ImposedPressure`: the inner pressure is specified by the user. This
  is default.
- `TightPipe`: the total amount of substance of the gas is constant.
- `ImposedOuterRadius`: the evolution of the outer radius is specified
  by the user.
- `ImposedInnerRadius`: the evolution of the inner radius is specified
  by the user.

Note that the modeling of a mandrel may affect the axial loading once
the pipe and the mandrel are in contact. Modeling of a mandrel is
detailed below in a dedicated section.

## Imposed inner pressure loading

## Imposed outer pressure loading

## Imposed inner radius loading

`PTest` now supports imposing the evolution of the inner radius of the
pipe. In this case, `PTest` adds the inner pressure as an unknown and
reports it in the output file.

### Example of usage

The following code shows how to impose the evolution of the inner radius
of the pipe:

~~~~{.cxx}
@RadialLoading 'ImposedInnerRadius';
@InnerRadiusEvolution{0 : 4.2e-3, 1 : 4.3e-3};
~~~~

## Tight pipe

`MTest` is able to model a tight pipe, which assumes that the amount of
substance of the gas inside the pipe is constant. The evolution of the
inner pressure is linked to the temperature and radius evolution through
the gas equation of state. By default, the equation of state of a
perfect gas is used.

### Specifying the gas equation of state

The equation of state of the gas can be specified using the
`@GasEquationOfState` keyword.

This keywords expects a function \(f\paren{P,V,T}\) such that the
equation of state is:

\[
f\paren{P,V,T, n}=0
\]{#eq:GasEquationOfState}

where:

- \(P\) is the inner pressure in the pipe.
- \(V\) is the volume of gas in a pipe of unit height. For small strain
  analyses, \(V\) is equal to the initial volume of the pipe. For finite
  strain analysis, \(V\) is computed by:
  \[
  V = \pi \, \ets{R}^{2} \, \paren{1 + \ets{\varepsilon_{zz}}}
  \]
  where \(\ets{R}\) and \(\ets{\varepsilon_{zz}}\) are respectively the
  current estimation of the radius and the current estimation of the
  linear axial strain at the end of the time.
- \(T\) is the current value of temperature at the inner surface of the
  pipe (the temperature is assumed homogeneous inside the pipe).
- \(n\) is the amount of substance of the gas. This quantity is assumed
  constant during the experiment. Its value is deduced from Equation
  @eq:GasEquationOfState by using:
  - The filling pressure (see the `@FillingPressure` keyword).
  - The filling temperature (see the `@FillingTemperature` keyword).
  - The initial inner radius of the pipe[^1].

[^1]: Using the initial inner radius of the pipe generally endows an
  approximation as the initial radius is usually known before the gas is
  introduced in the pipe.

#### A first example

In this example, the equation of state of a perfect gas is specified:

~~~~{.cxx}
@FillingPressure      1.e5;
@FillingTemperature 293.15;
@Real 'R' 8.314;
@GasEquationOfState 'P*V-n*R*T';
~~~~

#### A second example

In this example, the equation of state of a perfect gas is specified,
but one can consider a case where the gas only occupies a fraction
\(\alpha\,V\) of the volume inside the pipe:

~~~~{.cxx}
@FillingPressure      1.e5;
@FillingTemperature 293.15;
@Real 'R' 8.314;
@Real 'a' '0.8';
@GasEquationOfState 'P*a*V-n*R*T';
~~~~

# Axial loadings

The `AxialLoading` keyword let the user choose the kind of loading to
be applied to the pipe in the axial direction.

This keyword is followed by a string. The following loadings are
supported:

- `None`: no loading is applied to the pipe.
- `EndCapEffect`: loading applied to the pipe model the inner and outer
  pressure effects on a closed pipe. This is the default.
- `ImposedAxialForce`: the axial force acting of the pipe is specified
  by the user.
- `ImposedAxialGrowth`: the axial growth of the pipe is specified by
  the user.

Note that the modeling of a mandrel may affect the axial loading once
the pipe and the mandrel are in contact. Modeling of a mandrel is
detailed below in a dedicated section.

## End-cap effect


## Imposed axial growth

See the `@AxialGrowthEvolution` for details.

## Imposed axial force

See the `@AxialForceEvolution` for details.

# Modeling a non-deformable mandrel inside the pipe

The effect of a mandrel located inside the pipe can be modelled by
defining the evolution of its radius \(R_{m}\) using the
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

## Axial binding

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

# Support for failure criteria for pipes

Failure criteria can be added to pipe modelling using the
`@FailureCriterion` keyword. Note that no failure criterion is currently
shipped with `MTest`.

A failure criterion is called at the end of each time step to detect
failure of the pipe.

Each failure criterion adds a column to the output file giving the
status of the criterion:

- `0` means that the criterion is not met, i.e. no failure is detected
  and the pipe is sound.
- `1` means that the criterion is met, i.e. failure is detected and the
  pipe is broken.

In case of failure, three policies can be selected using the
`@FailurePolicy` keyword:

- `ReportOnly`: failure does not affect computation. The evaluation of
  the failure criteria only affects the output file.
- `StopComputation`: failure leads to reject the current time step. If
  substepping is enabled, the time step is divided by two. With this
  policy, one can thus only approach the failure time, but never go
  beyond.
- `FreezeState` (or `FreezeStateUntilEndOfComputation`): if a failure is
  detected, the state of the structure is frozen and does not evolve. No
  equilibrium is performed, the behaviour is no more called and `PipeTest`
  will output the same results again and again until the end of
  computation. This option may be useful when optimizing material
  parameters.

## Example of usage

~~~~{.cxx}
@FailurePolicy 'FreezeState';
@FailureCriterion 'ElongationAtBreak' {maximum_value : 1e-4};
~~~~

# Support for oxidation models for pipes

The `@OxidationModel` keyword introduces a model which computes an
oxidation length at either the inner boundary or the outer boundary of
the pipe. This keyword must be followed by a data map with the following
entries:

- `model`: the name of model.
- `library`: the name of the library in which the model is available.
- `boundary`: the name of boundary on which the model is defined. This
  variable must be equal to `inner_boundary` or `outer_boundary`.

An oxidation model must define an internal state variable named
`OxidationLength`. The values of the material properties and external
state variables passed to an oxidation model are computed on the
boundary on which the model is defined.

The definition of at least one oxidation model automatically defines an
evolution named `OxidationStatus` which states if an integration point
is inside an oxidation layer.

## Example of usage

~~~~{.python}
@OxidationModel{
  model : 'umatm5deziroxoxidationmodel_srma2020b',
  library : 'src/libUmatM5.so',
  boundary : 'outer_boundary'
};
~~~~

# Post-processings

## The `computeIntegralValue` and `computeMeanValue` post-processings

The `computeIntegralValue` and `computeMeanValue` post-processings
allows calculating the integral and the average of a scalar value in the
thickness of the pipe. Those functions allows to calculating those
quantities in the current or initial configurations

### Example of usage

~~~~{.cxx}
@AdditionalOutputs {'mean_value_initial_configuration':'SRR',
                    'mean_value_current_configuration':'SRR'}; // compute mean values of SRR
@AdditionalOutputs {'integral_value_initial_configuration':'SRR',
                    'integral_value_current_configuration':'SRR'}; // compute integral values of SRR
~~~~