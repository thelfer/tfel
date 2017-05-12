% New functionalities of the 3.1 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2017

\newcommand{\Frac}[2]{\displaystyle\frac{\displaystyle #1}{\displaystyle #2}}
\newcommand{\paren}[1]{\left(#1\right)}
\newcommand{\deriv}[2]{\Frac{\partial #1}{\partial #2}}
\newcommand{\tenseur}[1]{\underline{#1}}

The page declares the new functionalities of the 3.1 version of
`TFEL`, `MFront` and `MTest`.

# Highlights

## Numerical stability

#### Tests with `verrou`

### Enabling the `-ffast-math` with GCC

# TFEL

## TFEL/Utilities

### String algorithms

#### The `starts_with` string algorithm

The `starts_with` string algorithm is an helper function used to
determine if a given string starts with another.

#### The `ends_with` string algorithm

The `ends_with` string algorithm is an helper function used to
determine if a given string ends with another.

## TFEL/System

#### The `LibraryInformation` class

This release introduces the `LibraryInformation` class that allow
querying a library about exported symbols.

> **Note** This class has been adapted from the `boost/dll` library
> version 1.63 and has been originally written by Antony Polukhin,
> Renato Tegon Forti and Antony Polukhin.

#### Improvements to the `ExternalLibraryManager` class

##### Completion of libraries names

If a library is not found, the `ExternalLibraryManager` class will
try the following combinaisons:

- Append `lib` in front of the library name (except for `Microsoft`
  `Windows` platforms).
- Append `lib` in front of the library name and the standard library
  suffix at the end (except for `Microsoft` `Windows` platforms).
- Append the standard library suffix at the end of the library name.

The standard library suffix is:

- `.dll` for `Microsoft` `Windows` platforms.
- `.dylib` for `Apple` `MacOs` plateforms.
- `.so` on all other supported systems.

##### Retrieving the path of a library

The `getLibraryPath` method returns the path to a shared library:

- The method relies on the `GetModuleFileNameA` function on `Windows`
  which is reliable.
- On `Unix`, no portable way exists, so the method simply looks if the
  library can be loaded. If so, the method look if the file exists
  locally or in a directory listed in the `LD_LIBRARY_PATH` variable. 

##### Better handling of behaviour parameters

The `ExternalLibraryManager` class has several new methods for better
handling of behaviours' parameters:

- The `getUMATParametersNames` returns the list of parameters.
- The `getUMATParametersTypes` returns a list of integers which gives
  the type of the associated paramater: The integer values returned
  have the following meaning:
    - 0: floatting point value
    - 1: integer value
    - 2: unsigned short value
- The `getRealParameterDefaultValue`,
  `getIntegerParameterDefaultValue`, and
  `getUnsignedShortParameterDefaultValue` methods allow retrieving the
  default value of a parameter.

#### Retrieving bounds values

The `ExternalLibraryManager` class has several new methods for better
handling of a behaviour' variable bounds:

- The `hasBounds`,`hasLowerBound` and `hasUpperBound` allow querying
  about the existence of bounds for a given variable.
- The `getLowerBound` method returns the lower bound a variable, if defined.
- The `getUpperBound` method returns the upper bound a variable, if defined.

#### Retrieving physical bounds values

The `ExternalLibraryManager` class has several new methods for better
handling of a behaviour' variable bounds:

- The `hasPhysicalBounds`,`hasLowerPhysicalBound` and
  `hasUpperPhysicalBound` allow querying about the existence of bounds
  for a given variable.
- The `getLowerPhysicalBound` method returns the physical lower bound
  a variable, if defined.
- The `getUpperPhysicalBound` method returns the physical upper bound
  a variable, if defined.

##### Retrieving all mfront generated entry points and associated information

The `getEntryPoints` method returns a list containing all mfront
generated entry points. Those can be functions or classes depending on
the interface's needs.

The `getMaterialKnowledgeType` allows retrieving the material
knowledge type associated with and entry point. The returned value has the following meaning:

- 0: Material property.
- 1: Behaviour.
- 2: Model.

The `getInterface` method allows retrieving the interface of used to
generate an entry point.

The following code retrieves all the behaviours generated with the
`aster` interface in the `libAsterBehaviour.so` library:

~~~~{.cpp}
auto ab = std::vector<std::string>{};
const auto l = "AsterBehaviour";
auto& elm = ExternalLibraryManager::getExternalLibraryManager();
for(const auto& e : elm.getEntryPoints(l)){
  if((elm.getMaterialKnowledgeType(l,e)==1u)&&(elm.getInterface(l,e)=="Aster")){
	ab.push_back(e);
  }
}
~~~~

Note that we did not mention the prefix and the suffix of the
library. The library path is searched through the `getLibraryPath`
method.

The equivalent `python` code is the following:

~~~~{.python}
ab  = []
l   = 'AsterBehaviour';
elm = ExternalLibraryManager.getExternalLibraryManager();
for e in elm.getEntryPoints(l):
	if ((elm.getMaterialKnowledgeType(l,e)==1) and
		(elm.getInterface(l,e)=='Aster')):
		ab.append(e)
~~~~

## TFEL/Math

### Symmetric tensor eigen values and eigen vectors

The computation of the eigen values and eigen vectors of a symmetric
tensor has been improved in various ways:

- Various overloaded versions of the `computeEigenValues`,
  `computeEigenVectors` and `computeEigenTensors` methods have been
  introduced for more readable usage and compatibility with
  *structured bindings* construct introduced in C++17: the results of
  the computations are returned by value. There is also a new optional
  parameter allowing to sort the eigen values.
- New eigen solvers have been introduced.

#### New overloaded versions of the `computeEigenValues`, `computeEigenVectors` and `computeEigenTensors` methods 

##### Return by value

Various overloaded versions of the `computeEigenValues`,
`computeEigenVectors` and `computeEigenTensors` methods have been
introduced for more readable usage and compatibility with *structured
bindings* construct introduced in C++17: the results of the
computations are returned by value.

For example:

~~~~~{.cpp}
tmatrix<3u,3u,real> m2;
tvector<3u,real>    vp2;
std::tie(vp,m)=s.computeEigenVectors();
~~~~~

Thanks to C++17 *structured bindings* construct, the previous code
will be equivalent to this much shorter and more readable code:

~~~~~{.cpp}
auto [vp,m] = s.computeEigenVectors();
~~~~~

Even better, we could write:

~~~~~{.cpp}
const auto [vp,m] = s.computeEigenVectors();
~~~~~

##### Eigen values sorting

The `computeEigenValues` and `computeEigenVectors` methods now have an
optional argument which specify if we want the eigen values to be
sorted. Three options are available:

- `ASCENDING`: the eigen values are sorted from the lowest to the greatest.
- `DESCENDING`: the eigen values are sorted from the greatest to the lowest.
- `UNSORTED`: the eigen values are not sorted.

Here is how to use it:

~~~~~{.cpp}
tmatrix<3u,3u,real> m2;
tvector<3u,real>    vp2;
std::tie(vp,m)=s.computeEigenVectors(Stensor::ASCENDING);
~~~~~

#### New eigen solvers

The default eigen solver for symmetric tensors used in `TFEL` is based
on analitical computations of the eigen values and eigen vectors. Such
computations are more efficient but less accurate than the iterative
Jacobi algorithm (see [@kopp_efficient_2008;@kopp_numerical_2017]).

With the courtesy of Joachim Kopp, we have created a `C++11` compliant
version of his routines that we gathered in header-only library called
`FSES` (Fast Symmetric Eigen Solver). This library is included with
`TFEL` and provides the following algorithms:

- Jacobi
- QL with implicit shifts
- Cuppen
- Analytical
- Hybrid
- Householder reduction

We have also introduced the Jacobi implementation of the `Geometric`
`Tools` library (see [@eberly_robust_2016;@eberly_geometric_2017]).

Those algorithms are available in 3D. For 2D symmetric tensors, we
fall back to some default algorithm as described below.

| Name                        | Algorithm  in 3D          | Algorithm  in 2D   |
|:---------------------------:|:-------------------------:|:------------------:|
| `TFELEIGENSOLVER`           | Analytical (TFEL)         | Analytical (TFEL)  |
| `FSESJACOBIEIGENSOLVER`     | Jacobi                    | Analytical (FSES)  |
| `FSESQLEIGENSOLVER`         | QL with implicit shifts   | Analytical (FSES)  |
| `FSESCUPPENEIGENSOLVER`     | Cuppen's Divide & Conquer | Analytical (FSES)  |
| `FSESANALYTICALEIGENSOLVER` | Analytical			      | Analytical (FSES)  |
| `FSESHYBRIDEIGENSOLVER`     | Hybrid				      | Analytical (FSES)  |
| `GTESYMMETRICQREIGENSOLVER` | Symmetric QR              | Analytical (TFEL)  |
: List of available eigen solvers. {#tbl:eigensolvers}

The various eigen solvers available are enumerated in Table
@tbl:eigensolvers.

The eigen solver is passed as a template argument of the
`computeEigenValues` or the `computeEigenVectors` methods as
illustrated in the code below:

~~~~~{.cpp}
tmatrix<3u,3u,real> m2;
tvector<3u,real>    vp2;
std::tie(vp,m)=s.computeEigenVectors<Stensor::GTESYMMETRICQREIGENSOLVER>();
~~~~~

|  Algorithm                  |  Failure ratio  | \(\Delta_{\infty}\) |   Times (ns)  |  Time ratio |
|:---------------------------:|:---------------:|:-------------------:|:-------------:|:-----------:|
| `TFELEIGENSOLVER`           |   0.000642      | 3.29e-05            |   250174564	  |	1	  	    |
| `GTESYMMETRICQREIGENSOLVER` |   0             | 1.10e-06            |   359854550   |	1.44	    |
| `FSESJACOBIEIGENSOLVER`     |   0             | 5.62e-07            |   473263841   |	1.89	    |
| `FSESQLEIGENSOLVER`         |   0.000397      | 1.69e-06            |   259080052   |	1.04	    |
| `FSESCUPPENEIGENSOLVER`     |   0.019469      | 3.49e-06            |   274547371   | 1.10	    |
| `FSESHYBRIDEIGENSOLVER`     |   0.076451      | 5.56e-03            |   126689850   |	0.51	    |
| `FSESANALYTICALEIGENSOLVER` |   0.108877      | 1.58e-01            |   127236908   |	0.51  	    |
: Test on \(10^{6}\) random symmetric tensors in single precision (`float`). {#tbl:comp_eigensolvers_float}

|  Algorithm                  |  Failure ratio  | \(\Delta_{\infty}\) |   Times (ns)  |  Time ratio |
|:---------------------------:|:---------------:|:-------------------:|:-------------:|:-----------:|
| `TFELEIGENSOLVER`           |   0.000632      | 7.75e-14            |   252663338   |	1		    |
| `GTESYMMETRICQREIGENSOLVER` |   0             | 2.06e-15            |   525845499   |	2.08	    |
| `FSESJACOBIEIGENSOLVER`     |   0             | 1.05e-15            |   489507133   |	1.94	    |
| `FSESQLEIGENSOLVER`         |   0.000422      | 3.30e-15            |   367599140   |	1.45	    |
| `FSESCUPPENEIGENSOLVER`     |   0.020174      | 5.79e-15            |   374190684   |	1.48	    |
| `FSESHYBRIDEIGENSOLVER`     |   0.090065      | 3.53e-10            |   154911762   |	0.61	    |
| `FSESANALYTICALEIGENSOLVER` |   0.110399      | 1.09e-09            |   157613994   |	0.62	    |
: Test on \(10^{6}\) random symmetric tensors in double precision (`double`). {#tbl:comp_eigensolvers_double}

|  Algorithm                  |  Failure ratio  | \(\Delta_{\infty}\) |   Times (ns)  |  Time ratio |
|:---------------------------:|:---------------:|:-------------------:|:-------------:|:-----------:|
| `TFELEIGENSOLVER`           |   0.000575      |  2.06e-17           |   428333721   |	1		    |
| `GTESYMMETRICQREIGENSOLVER` |   0             |  1.00e-18           |   814990447   |	1.90	    |
| `FSESJACOBIEIGENSOLVER`     |   0             |  5.11e-19           |   748476926   |	1.75	    |
| `FSESQLEIGENSOLVER`         |   0.00045       |  1.83e-18           |   548604588   |	1.28	    |
| `FSESCUPPENEIGENSOLVER`     |   0.009134      |  3.23e-18           |   734707748   |	1.71	    |
| `FSESHYBRIDEIGENSOLVER`     |   0.99959       |  4.01e-10           |   272701749   |	0.64	    |
| `FSESANALYTICALEIGENSOLVER` |   0.999669      |  1.36e-11           |   315243286   |	0.74	    |
: Test on \(10^{6}\) random symmetric tensors in extended precision (`long double`). {#tbl:comp_eigensolvers_long_double}

#### Some benchmarks

We have compared the available algorithm on \(10^{6}\) random
symmetric tensors whose components are in \([-1:1]\).

For a given symmetric tensor, we consider that the computation of the
eigenvalues and eigenvectors failed if:
\[
\Delta_{\infty}=\max_{i\in[1,2,3]}\left\|\tenseur{s}\,\cdot\,\vec{v}_{i}-\lambda_{i}\,\vec{v}_{i}\right\|>10\,\varepsilon
\]
where \(\varepsilon\) is the accuracy of the floatting point considered.

The results of those tests are reported on Tables
@tbl:comp_eigensolvers_float, @tbl:comp_eigensolvers_double and
@tbl:comp_eigensolvers_long_double:

- The standard eigen solver available in previous versions of `TFEL`
  offers a very interesting compromise between accuracy and numerical
  efficiency.
- If very accurate results are required, the `FSESJACOBIEIGENSOLVER`
  eigen solver is a good choice.

### Isotropic functions and Isotropic function derivatives of symmetric tensors

Given a scalar valuated function \(f\), one can define an associated
isotropic function for symmetric tensors as:
\[
f\paren{\tenseur{s}}=\sum_{i=1}^{3}f\paren{\lambda_{i}}\tenseur{n}_{i}
\]

where \(\left.\lambda_{i}\right|_{i\in[1,2,3]}\) are the eigen values
of the symmetric tensor \(\tenseur{s}\) and
\(\left.\tenseur{n}_{i}\right|_{i\in[1,2,3]}\) the associated eigen
tensors.

If \(f\) is \(\mathcal{C}^{1}\), then \(f\) is a differentiable
function of \(\tenseur{s}\).

\(f\) can be computed with the `computeIsotropicFunction` method of
the stensor class. \(\deriv{f}{\tenseur{s}}\) can be computed with
`computeIsotropicFunctionDerivative`. One can also compute \(f\) and
\(\deriv{f}{\tenseur{s}}\) all at once by the
`computeIsotropicFunctionAndDerivative` method. All those methods are
templated by the name of the eigen solver (if no template parameter is
given, the `TFELEIGENSOLVER` is used).

Various new overloaded versions of those methods have been introduced
in `TFEL-3.1`. This overloaded methods are meant to:

- allow the user to explicitly give the values of \(f\) or \(df\),
  rather than the functions to compute them. This allows to reduce the
  computational cost of the evaluation of the isotropic function when
  the values of the derivatives can directly be computed from the
  values of \(f\). See the example \(\exp\) example below.
- return the results by value. This allow a much more readable code if
  the *structure bindings* feature of the C++17 standard is available.

To illustrate this new features, assuming that the the *structure
bindings* feature of the C++17 standard is available, one can now
efficiently evaluate the exponential of a symmetric tensor and its
derivative as follows:

~~~~{.cpp}
const auto [vp,m] = s.computeEigenVectors();
const auto evp    = map([](const auto x){return exp(x)},vp);
const auto [f,df] = Stensor::computeIsotropicFunctionAndDerivative(evp,evp,vp,m,1.e-12);
~~~~

### Portable implementation of the `fpclassify`, `isnan`,  `isfinite` functions

The `C99` standard defines the `fpclassify`, `isnan`, `isfinite`
functions to query some information about double precision
floatting-point numbers (`double`):

- Following the `IEEE754` standard, the `fpclassify` categorizes a
  floating point number into one of the following categories: zero,
  subnormal, normal, infinite, NaN (Not a Number). The return value
  returned for each category is respectively `FP_ZERO`,
  `FP_SUBNORMAL`, `FP_NORMAL`, `FP_INFINITE` and `FP_NaN`.
- The `isnan` function returns a boolean stating if its argument has a
  not-a-number (NaN) value.
- The `isfinite` function returns true if its argument falls into one
  of the following categories: zero, subnormal or normal.

The `C++11` provides a set of overload for single precision (`float`)
and extended precision (`long double`) floatting-point numbers.

Those functions are very handy to check the validity of a
computation. However, those functions are not compatible with the use
of the `-ffast-math` option of the `GNU` compiler which also implies
the `-ffinite-math-only` option. This latter option allows
optimizations for floating-point arithmetic that assume that arguments
and results are finite numbers. As a consequence, when this option is
enabled, the previous functions does not behave as expected. For
example, `isnan` always returns false, whatever the value of its
argument.

To overcome this issue, we have introduced in `TFEL/Math` the
implementation of these functions provided by the `musl` library (see
@musl_libc_2017). Those implementations are compatible with the
`-ffast-math` option of the `GNU` compiler. Those implementations are
defined in the `TFEL/Math/General/IEEE754.hxx` header file in the
`tfel::math::ieee754` namespace.

# New functionalities of the `MFront` code generator

## Gallery

The `MFront` gallery is meant to present well-written implementation of
behaviours that will be updated to follow `MFront` latest evolutions.

### Hyperelastic behaviours

The implementation of various hyperelastic behaviours can be found
[here](hyperelasticity.html):

- [Signorini](signorini.html)
- [Ogden](ogden.html)

### Hyperviscoelastic behaviours

The following page describes how to implement standard
hyperviscoelastic behaviours based on the development in Prony series:

<http://tfel.sourceforge.net/hyperviscoelasticity.html>

### Plasticity

An example of a simple orthotropic behaviour is available
[here](orthotropiclinearhardeningplasticity.html).

### Viscoplasticity

This following article shows how to implement an isotropic
viscoplastic behaviour combining isotropic hardening and multiple
kinematic hardenings following an Armstrong-Frederic evolution of the
back stress:

<http://tfel.sourceforge.net/isotropicplasticityamstrongfrederickinematichardening.html>

## Behaviours interfaces

### The `Cast3M` interface

#### The `MieheApelLambrechtLogarithmic` finite strain strategy

The pre- and post-computations performed by the
`MieheApelLambrechtLogarithmic` finite strain strategy , which require
the computation of the eigen values and eigen vectors of the right
Cauchy strecth tensor, are now based the Jacobi algorithm from the
`FSES` library for improved accuracy.

### The `Code_Aster` interface

#### Support for the `GROT_GDEP` finite strain formulation

`GROT_GDEP` is the name in `Code_Aster` of a finite strain formulation
based on the principle of virtual work in the reference configuration
expressed in term of the Green-Lagrange strain and the second
Piola-Kirchhoff stress. Such a formulation is also called `Total
Lagrangian` in the litterature (see @belytschko_nonlinear_2000) and in
other finite element solvers.

Prior to this version, `MFront` behaviours were meant to be used with
the `SIMO_MIEHE` finite strain formulation and could not be used with
the `GROT_GDEP` finite strain formulation.

From the behaviour point of view, using `SIMO_MIEHE` or `GROT_GDEP`
differs from the choice of the output stress and the definition of the
consistent tangent operator.

#### The `@AsterFiniteStrainFormulation` keyword

The `@AsterFiniteStrainFormulation` keyword can now be used to choose
one of these finite strain formulation.

This keyword must be followed by one of the following choice:

- `SIMO_MIEHE`
- `GROT_GDEP` or `TotalLagrangian`

The choice `SIMO_MIEHE` remains the default for backward
compatibility.

### The `Europlexus` interface

#### The `MieheApelLambrechtLogarithmic` finite strain strategy

The pre- and post-computations performed by the
`MieheApelLambrechtLogarithmic` finite strain strategy, which require
the computation of the eigen values and eigen vectors of the right
Cauchy strecth tensor, are now based the Jacobi algorithm from the
`FSES` library for improved accuracy.

### The `Abaqus-Explicit` interface

#### The `MieheApelLambrechtLogarithmic` finite strain strategy

The pre- and post-computations performed by the
`MieheApelLambrechtLogarithmic` finite strain strategy, which require
the computation of the eigen values and eigen vectors of the right
Cauchy strecth tensor, are now based the Jacobi algorithm from the
`FSES` library for improved accuracy.

# New functionalities of `MTest` solver

## Choice of the rounding mode from the command line

\(4\) rounding mode are defined in the IEEE754 standard. Changing the
rounding mode is a gross way to check the numerical stability of the
computations performed with `MTest` and `MFront`.

The rounding mode can be set using the `--rounding-direction-mode`
option. Valid values for this option are:

- `DownWard`: Round downward.
- `ToNearest`: Round to nearest (the default).
- `TowardZero`: Round toward zero.
- `UpWard`: Round upward.
- `Random`: rounding mode is changed randomly a various stage of the
  computation to one of the four previous rounding modes.

Most unit-tests based on `MTest` are now executed five times, one for
each available choice of the rounding mode.

## Abritrary non linear constraints

Abritrary non linear constraints on driving variables and
thermodynamic forces can now be added using the `@NonLinearConstraint`
keyword.

> **Note**
>
> This keyword can also be used to define linear constraints, although
> the numerical treatment of such a constraint will be sub-optimal. A
> special treatment of such a constraint is planned.

> **Note**
>
> This development of this functionality highlighted the issue
> reported in Ticket #39.
> For more details, see: <https://sourceforge.net/p/tfel/tickets/39/>

### Normalisation policy

This keyword must be followed by an option giving the normalisation
policy. The normalisation policy can have one of the following values:

- `DrivingVariable`, `Strain`, `DeformationGradient`,
  `OpeningDisplacement` stating that the constraint is of the order of
  magnitude of the driving variable.
- `ThermodynamicForce`, `Stress`, `CohesiveForce` stating that the
  constraint is of the order of magnitude of the thermodynamic force.

### Examples

~~~~~ {.cpp}
// ensure that the loading is isochoric in 1D
@NonLinearConstraint<Strain> 'FRR*FTT*FZZ-1';
~~~~~~~~~~

~~~~~ {.cpp}
// impose the first piola kirchoff stress
// in an uniaxial compression test
@Real 'Pi0' -40e6
@NonLinearConstraint<Strain> 'SXX*FYY*FZZ-Pi0';
~~~~~~~~~~

### `python` bindings

#### The `Behaviour` class

The `Behaviour` class has been introduced in the `mtest` modules. This
class can be used to determine at runtime time the material
properties, internal state variables, parameters and external state
variables required by a specific implementation.

Contrary the `tfel.system.ExternalBehaviourDescription` class, the
information given by the `Behaviour` class takes into account the
variables that are implicitly declared by the interface to match its
specific (internal) requirements. For example:

- The `castem` interface usually adds additional material properties
  describing the thermo-elastic properties. Such properties are may be
  unused by the behaviour.
- The `abaqus` interface may declare additional state variables to
  describe the orthotropic axes (this is mandatory for finite strain
  ortotropic behaviours).
- etc...

#### The `MTest` class

In the `python` bindings, the `setNonLinearConstraint` method has been
added to the `MTest` class.

This method takes two named arguments:

- `constraint`, the equation to be satified
- `normalisation_policy`. The normalisation policy can have one of the
  following values:
    - `DrivingVariable`, `Strain`, `DeformationGradient`,
	  `OpeningDisplacement` stating that the constraint is of the
	  order of magnitude of the driving variable
	- `ThermodynamicForce`, `Stress`, `CohesiveForce` stating that the
	  constraint is of the order of magnitude of the thermodynamic
	  force

# Tickets fixed

## Ticket #40:  `ImplicitDSL`: Detect non finite values during resolution

During the resolution of the implicit system, invalid results may
occur. In previous versions, no check were made leading to a
propagation of those values and finally the failure of integration.

A test to check that the residual of the implicit system is finite
have been added. If this test is not satisfied after the first
iteration, the last increment of the unknowns is divided by two and
the resolution is restarted with this guess. If this test is not
satisfied at the first iteration, the behaviour integration can not be
performed.

## Ticket #41: `MTest`: check if the residual is finite and not NaN

In previous versions, if the behaviour integration returned a
not-a-number value (`NaN` ), this value propagated throughout the
computation.

This situation can be detected by checking that the convergence
criteria are finite as defined by the `IEEE754` standard.

For more details, see: <https://sourceforge.net/p/tfel/tickets/41/>

## Ticket #42: Check for infinite and `NaN` values in material properties

In the previous versions of `MFront`, generated sources for material
properties checked that the `errno` value to determine is something
had gone wrong, but this check does not appear to portable nor
reliable with the INTEL compiler or when the `-ffast-math` option of
the GNU compiler is activated.

The current version now check that the return value is finite.

For more details, see: <https://sourceforge.net/p/tfel/tickets/42/>

## Ticket #43: Add the list of parameters' names and types to generated library for the `UMAT++` interface

In previous versions,, the list of parameters' names and types were
not exported to the generated library for the `UMAT++` interface,
i.e. the additional symbols defined in the generated shared libraries
that can be read through the `ExternalLibraryManager` class.

For more details, see: <https://sourceforge.net/p/tfel/tickets/43/>

## Ticket #45: Support for bounds on parameters

For more details, see: <https://sourceforge.net/p/tfel/tickets/45/>

## Ticket #46: Improvements to the `mfront` `python` module

The following improvements to the `mfront` `python` module have been
made:

- Add missing metods in the `BehaviourDescription` class to retrieve
  information about the material symmetry
- Add missing methods to retrieve information about standard and
  physical variables' bounds.

For more details, see: <https://sourceforge.net/p/tfel/tickets/46/>

## Ticket #47: Add `python` bindings for the `mtest::Behaviour` class

The `mtest` module now contains bindings for the `mtest::Behaviour`
class. This class allow querying information about how to use a
behaviour in a specific context (interface and modelling hypothesis):
for example, if a behaviour has the `requireStiffnessTensor`
attribute, the list of material properties is updated appropriately if
required by the interface for the considered modelling hypothesis. The
`Behaviour` class has the following useful methods:

- `getBehaviourType`: Return the behaviour type.
- `getBehaviourKinematic`: Return the behaviour kinematic.
- `getDrivingVariablesSize`: Return the size of a vector able to
	 contain all the components of the driving variables.
- `getThermodynamicForcesSize`: Return the size of a vector able to
	 contain all the components of the thermodynamic forces.
- `getStensorComponentsSuffixes`: Return the components suffixes of a
  symmetric tensor.
- `getVectorComponentsSuffixes`: Return the components suffixes of a
  vector.
- `getTensorComponentsSuffixes`: Return the components suffixes of a
  tensor.
- `getDrivingVariablesComponents`: Return the components of the
  driving variables.
- `getThermodynamicForcesComponents`: Return the components of the
  thermodynamic forces.
- `getDrivingVariableComponentPosition`: Return the position of the
  component of a driving variable.
- `getThermodynamicForceComponentPosition`: Return the position of the
  component of a thermodynamic force.
- `getSymmetryType`: Return the symmetry of the behaviour:
    -- 0 means that the behaviour is isotropic.
    -- 1 means that the behaviour is orthotropic.
- `getMaterialPropertiesNames`: return the names of the material
  properties.
- `getInternalStateVariablesNames`: Return the names of the internal
  state variables.
- `expandInternalStateVariablesNames`: Return the names of the
  internal state variables, taking into account the suffixes for
  vectors, symmetric tensors and tensors.
- `getInternalStateVariablesSize`: Return the the size of the array of
  internal variables.
- `getInternalStateVariablesDescriptions`: Return the descriptions the
  internal variables.
- `getInternalStateVariableType`: Return the type of an internal
  variable:
    - 0 means that the internal state variable is a scalar.
    - 1 means that the internal state variable is a symmetric tensor.
    - 3 means that the internal state variable is a tensor\n.
- `getInternalStateVariablePosition`: Return the internal state
  variable position.
- `getExternalStateVariablesNames`: Return the names of the external
  state variables.
- `getParametersNames`: Return the names of the floating point
  parameters.
- `getIntegerParametersNames`: Return the names of the integer
  parameters.
- `getUnsignedShortParametersNames`: Return the names of the unsigned
  short parameters.
- The `getRealParameterDefaultValue`,
  `getIntegerParameterDefaultValue` and
  `getUnsignedShortParameterDefaultValue` methods can be used to
  retrieve the default value of a parameter.
- The `hasBounds` method returns true if the given variable has
  bounds.
- The `hasLowerBound` method returns true if the given variable has a
  lower bound.
- The `hasUpperBound` method hasUpperBound returns true if the given
  variable has an upper bound.
- The `getLowerBound` method returns the lower bound of the given
  variable.
- The `getUpperBound` method returns the uppert bound of the given
  variable.
- The `hasPhysicalBounds` methodreturns true if the given variable has
  physical bounds.
- The `hasLowerPhysicalBound` method returns true if the given
  variable has a physical lower bound.
- The `hasUpperPhysicalBound` method returns true if the given
  variable has a physical upper bound.
- The `getLowerPhysicalBound` method returns the lower bound of the
  given variable.
- The `getUpperPhysicalBound` method returns the upper bound of the
  given variable.

For more details, see: <https://sourceforge.net/p/tfel/tickets/47/>

Here is an example of the usage of the `Behaviour` class in `python`:

~~~~{.python}
import mtest
b= mtest.Behaviour('AsterBehaviour','asternorton','Tridimensional');
for p in b.getParametersNames():
    print('- '+p+': '+str(b.getRealParameterDefaultValue(p)))
for p in b.getIntegerParametersNames():
    print('- '+p+': '+str(b.getIntegerParameterDefaultValue(p)))
for p in b.getUnsignedShortParametersNames():
	print('- '+p+': '+str(b.getUnsignedShortParameterDefaultValue(p)))
~~~~

## Ticket #46: Improved `python` bindings for the `mfront::BehaviourDescription` class

The `python` bindings of the `mfront::BehaviourDescription` now gives
access to the parameters default values, and information about a
variable standard or physical bounds (type, range).

Here is an example of its usage:

~~~~{.python}
from tfel.material import ModellingHypothesis
import mfront

def printBounds(n,b):
    print('Bounds of variable \''+n+'\':')
    if((b.boundsType==mfront.VariableBoundsTypes.LOWER) or
       (b.boundsType==mfront.VariableBoundsTypes.LOWERANDUPPER)):
        print('- lower bound: '+str(b.lowerBound))
    if((b.boundsType==mfront.VariableBoundsTypes.UPPER) or
       (b.boundsType==mfront.VariableBoundsTypes.LOWERANDUPPER)):
        print('- upper bound: '+str(b.upperBound))
        print('')

dsl = mfront.getDSL('Norton.mfront')
dsl.analyseFile('Norton.mfront',[])

# behaviour description
bd = dsl.getBehaviourDescription()

if(bd.getSymmetryType()==mfront.BehaviourSymmetryType.ISOTROPIC):
    print 'Isotropic behaviour\n'
else:
    print 'Orthropic behaviour\n'

if(bd.getElasticSymmetryType()==mfront.BehaviourSymmetryType.ISOTROPIC):
    print 'Isotropic elasticity\n'
else:
    print 'Orthropic elasticity\n'

# a deeper look at the 3D case
d = bd.getBehaviourData(ModellingHypothesis.TRIDIMENSIONAL)
for p in d.getParameters():
    if(p.arraySize==1):
        if(p.hasBounds()):
            printBounds(p.name,p.getBounds())
    else:
        for i in range(p.arraySize):
            if(p.hasBounds(i)):
				printBounds(p.name+'['+str(i)+']',p.getBounds(i))
~~~~
				
## Ticket #48: Add the ability to retrieve bounds for material properties and parameters from the `mtest::Behaviour` class

The following methods were added to the `mtest.Behaviour` class:
- The `hasBounds` method returns true if the given variable has
  bounds.
- The `hasLowerBound` method returns true if the given variable has a
  lower bound.
- The `hasUpperBound` method hasUpperBound returns true if the given
  variable has an upper bound.
- The `getLowerBound` method returns the lower bound of the given
  variable.
- The `getUpperBound` method returns the uppert bound of the given
  variable.
- The `hasPhysicalBounds` methodreturns true if the given variable has
  physical bounds.
- The `hasLowerPhysicalBound` method returns true if the given
  variable has a physical lower bound.
- The `hasUpperPhysicalBound` method returns true if the given
  variable has a physical upper bound.
- The `getLowerPhysicalBound` method returns the lower bound of the
  given variable.
- The `getUpperPhysicalBound` method returns the upper bound of the
  given variable.

Here is an example:

~~~~{.python}
from mtest import Behaviour

b = Behaviour('AsterBehaviour','asternorton','Tridimensional')

for p in b.getParametersNames():
    if b.hasLowerBound(p):
        print(p+" lower bound: "+str(b.getLowerBound(p)))
    if b.hasUpperBound(p):
        print(p+" lower bound: "+str(b.getUpperBound(p)))
~~~~
For more details, see: <https://sourceforge.net/p/tfel/tickets/48/>

## Ticket #49: Add the ability to retrieve the symmetry of the behaviour and the symmetry of the elastic behaviour from `mfront-query`

The following queries are now available:

- `--elastic-symmetry`: return the symmetry of the elastic part of the behaviour. If the returned value is 0, this part of the behaviour is isotropic. If the returned value is 1, this part of the behaviour is orthotropic.the behaviour is orthotropic.
- `--symmetry`: return the behaviour symmetry. If the returned value is 0, the behaviour is isotropic. If the returned value is 1, the behaviour is orthotropic.

For more details, see: <https://sourceforge.net/p/tfel/tickets/49/>

## Ticket #50: Add the ability to retrieve bounds values from `mfront-query`

The following queries are now available:

- `--has-bounds`: return `true` if a variable has bounds, `false` otherwise.
- `--bounds-type`: return the bounds type associated to a variable.
- `--bounds-value`: show the bounds value associated as a range.
- `--has-physical-bounds`: return `true` if a variable has physical bounds, `false` otherwise.
- `--physical-bounds-type`: return the physical bounds type associated to a variable.
- `--physical-bounds-value`: show the bounds value associated as a range.


For more details, see: <https://sourceforge.net/p/tfel/tickets/50/>

## Ticket #65: `@ElasticMaterialProperties` does not work for DSL describing isotropics behaviours

The `@ElasticMaterialProperties` is now available for domain specific
languages (DSL) describing isotropics behaviours.

~~~~{.cpp}
@DSL IsotropicStrainHardeningMisesCreep;
@Behaviour StrainHardeningCreep2;
@Author    Helfer Thomas;
@Date      23/11/06;

@ElasticMaterialProperties {"Inconel600_YoungModulus.mfront",0.3};

@MaterialProperty real A;
@MaterialProperty real Ns;
@MaterialProperty real Np;

@FlowRule{
  const real p0  = 1.e-6;
  const real tmp = A*pow(seq,Ns-1.)*pow(p+p0,-Np-1);
  f       = tmp*seq*(p+p0);
  df_dseq =  Ns*tmp*(p+p0);
  df_dp   = -Np*tmp*seq;
}
~~~~

For more details, see: <https://sourceforge.net/p/tfel/tickets/65/>

# References

<!-- Local IspellDict: english -->
