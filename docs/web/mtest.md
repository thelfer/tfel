% MTest: unit mechanical behaviour testing
% Helfer Thomas; Jean-Michel Proix
% August 19, 2014

\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}

# Overview

`MFront` comes with an handy easy-to-use tool called `MTest`. This
tool handles two types of computations:

- The description of a single material point.
- The description of a pipe, submitted to variaous kinds of loadings.

From various benchmarcks, this tool has been found much faster (from
ten to several hundred times depending on the test case) than using a
full-fledged finite element solver such as `Cast3M` or `Code_Aster`.

The `mtest` python module, described [here](mtest-python.html), allows
much finer control of the computations.

# Usage

MTest can be used from the command line as follows:

~~~~{.bash}
$ mtest [options] inputfile
~~~~

If the input file has the `ptest` extension, `MTest` will assume that
the input files describes a test on a pipe. Otherwise, a test on a
material point is assumed.

This can be changed by using the `--scheme` option that currently
accept two values:

- `--scheme=mtest` to specify that the simulation of a single material
  point is intended.
- `--scheme=ptest` to specify that the simulation of a pipe is
  intended.

## Getting help

### The `--help-keywords-list` command line option

The list of available keywords are available thanks to the
`--help-keywords-list` option. By default, this command describes the
list of keywords associated with the simulation of a single material
point.  To see the keywords associated with the simulation of a pipe,
use `--scheme=ptest` option *before* the `--help-keywords-list`
option.

### The `--help-keyword` option

The `--help-keyword` displays the help associated with a keyword. If
this keyword is specific to the simulation of a pipe, use
`--scheme=ptest` option *before* the `--help-keyword` option.

# Material point

`MTest` can test the local behaviour of a material, by imposing
independent constraints on each component of the driving variables (or
deformation gradient or or the stress. It equivalent to the
`SIMU_POINT_MAT` operator available within the `Code-Aster` finite
element solver [@edf_macro-commande_2013] or to the `SiDoLo` software
[@pilvin_sidolo_2003].

`MTest` can be used to model various experiments, as far as a stage
implying strain localisation is not reached: tensile, compressive or
shear tests driven by stresses or deformations, pipe loaded by
internal or external pressure, \nom{Satoh} test, etc.

`MTest` generates a text file containing the evolution of the strains
(for small strains behaviours), the stresses an the state variables
during the loading history. Other `MTest` functionalities include:

- the ability to test all the behaviours handled by `MFront` (small
  strain and finite strain behaviours, cohesive zone models);
-  the ability to test isotropic and orthotropic behaviours;
-  the support of various modelling hypotheses, notably the plane
  stress and axisymmetric generalised imposed plane stress
  hypotheses;
-  many features to evaluate the numerical performances of
  mechanical behaviours. For example, user can compare the computed
  tangent consistent operator to a numerical approximation;
- a C++ library and a Python interface
  [@von_rossum_python_2007]. `MTest` can be embedded in general purpose
  scientific environment to fit behaviour parameters against
  experimental data. In particular, `MTest` can be used in ADAO
  [@salome_adao_2014], a module for Data Assimilation and Optimization
  of the Salome platform [@salome_open_2014];
- comparison of the results (strains, stresses, internal state
  variables) to reference or analytical results. `MTest` automatically
  generates XML file using the [JUnit](http://junit.org) format. Those
  files can be used for reporting using the
  [Jenkins continuous integration application](http://jenkins-ci.org/)
  This functionality is central in the assurance quality procedure of
  `MFront`.

Through an appropriate option, a behaviour implementation generated
through `MFront` may create an `MTest` file in case of integration
failure: this `MTest` file only describe the failed time step with
the appropriate initial conditions. This feature is particularly
useful to analyse the failure of large simulations which may happen
after several hours of computations.

## Example

~~~~ {#MTestPlasticity .cpp .numberLines}
@Behaviour<aster> 'src/libAsterBehaviour.so' 'asterplasticity';
@MaterialProperty<constant> 'YoungModulus'     150.e9;
@MaterialProperty<constant> 'PoissonRatio'       0.3;
@MaterialProperty<constant> 'H'                100.e9;
@MaterialProperty<constant> 's0'               100.e6;
@ExternalStateVariable 'Temperature' {0:293.15,3600.:800};
@ImposedStrain<function> 'EXX' '1.e-3*t';
@Times {0.,1 in 20};
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Simulation of a pipe

`MTest` can also be used to model a pipe subjected to various loadings.

## Radial loadings

### Tight pipe

#### Specifying the gas equation of state

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

##### A first example

In this example, the equation of state of a perfect gas is specified:

~~~~{.cxx}
@FillingPressure      1.e5;
@FillingTemperature 293.15;
@Real 'R' 8.314;
@GasEquationOfState 'P*V-n*R*T';
~~~~

##### A second example

In this example, the equation of state of a perfect gas is specified,
but one imagine that the gas can only occupy a fraction \(\alpha\,V\) of
the volume inside the pipe:

~~~~{.cxx}
@FillingPressure      1.e5;
@FillingTemperature 293.15;
@Real 'R' 8.314;
@Real 'a' '0.8';
@GasEquationOfState 'P*a*V-n*R*T';
~~~~

## Axial loadings

## Modelling a non-deformable mandrel inside 

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

## Support for failure criteria for pipes

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
  detected, the state of the structure is freezed and do not evolve. No
  equilibrium is performed, the behaviour is no more called and `PipeTest`
  will output the same results again and again until the end of
  computation. This option may be useful when optimizing material
  parameters.

### Example of usage

~~~~{.cxx}
@FailurePolicy 'FreezeState';
@FailureCriterion 'ElongationAtBreak' {maximum_value : 1e-4};
~~~~

# References

<!-- Local IspellDict: english -->
