% New functionalities of the 3.4 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2020

\newcommand{\paren}[1]{{\left(#1\right)}}

The page declares the new functionalities of the 3.4 version of the
`TFEL` project.

# Documentation

[This
page](ExtendingStandardElastoViscoPlasticityBrick-StressCriterion.md)
describes how to extend the `TFEL/Material` library and the
`StandardElastoViscoPlasticity` brick with a new stress criterion.

# New features of the `TFEL` libraries

## New features of the `TFEL/Math` library

### Scalar Newton-Raphson algorithm

The function `scalarNewtonRaphson`, declared in the
`TFEL/Math/ScalarNewtonRaphson.hxx` is a generic implementation of the
Newton-Raphson algorithm for scalar non linear equations.

This implementation handles properly `IEEE754` exceptional cases
(infinite numbers, `NaN` values), even if advanced compilers options are
used (such as `-ffast-math` under `gcc`).

#### Usage

~~~~{.cxx}
// this lambda takes the current estimate of the root and returns
// a tuple containing the value of the function whose root is searched
// and its derivative.
auto fdf = [](const double x) {
  return std::make_tuple(x * x - 13, 2 * x);
};
// this lambda returns a boolean stating if the algorithm has converged
// the first argument is the value of the function whose root is searched
// the second argument is the Newton correction to be applied
// the third argument is the current estimate of the root
// the fourth argument is the current iteration number
auto c = [](const double f, const double, const double, const int) {
  return std::abs(f) < 1.e-14;
};
// The `scalarNewtonRaphson` returns a tuple containing:
// - a boolean stating if the algorithm has converged
// - the last estimate of the function root
// - the number of iterations performed
// The two last arguments are respectively the initial guess and 
// the maximum number of iterations to be performed
const auto r = tfel::math::scalarNewtonRaphson(fdf, c, 0.1, 100);
~~~~

# New features of `MFront`

## Coupling (visco-)plasticity with porosity evolutions in the `StandardElastoViscoPlasticity` brick

## The `StandardStressCriterionBase` and `StandardPorousStressCriterionBase` base class to  ease the extension of the `StandardElastoViscoPlasticity` brick

## Improved robustness of the isotropic DSLs

The `@Flow` block can now return a boolean value in the
`IsotropicMisesCreep`, `IsotropicMisesPlasticFlow`,
`IsotropicStrainHardeningMisesCreep` DSLs.

This allows to check to avoid Newton steps leading to too high values of
the equivalent stress by returning `false`. This usually occurs if in
elastic prediction is made (the default), i.e. when the plastic strain
increment is too low.

If `false` is returned, the value of the plastic strain increment is
increased by doubling the previous Newton correction. If this happens at
the first iteration, the value of the plastic strain increment is set to
half of its upper bound value (this upper bound value is such that the
associated von Mises stress is null).

## Generic behaviours

### Add the possibility of definining the tangent operator blocks: the `@TangentOperatorBlock` and `@TangentOperatorBlocks` keywords

In version `3.3.x`, some tangent operator blocks are automatically
declared, namely, the derivatives of all the fluxes with respect to all
the gradients. The `@AdditionalTangentOperatorBlock` and
`@AdditionalTangentOperatorBlocks` keywords allowed to add tangent
operator blocks to this default set.

The `@TangentOperatorBlock` and `@TangentOperatorBlocks` allow a more fine
grained control of the tangent operator blocks available and disable the
use of the default tangent operation blocks. Hence, tangent operator
blocks that are structurally zero (for example due to symmetry
conditions) don't have to be compute any more.

## Improvement of the `StandardElastoViscoplasticity` brick

### The `Power` isotropic hardening rule{#sec:power_isotropic_hardening_rule}

The `Power` isotropic hardening rule is defined by:
\[
R\paren{p}=R_{0}\,\paren{p+p_{0}}^{n}
\]

#### Options

The `Power` isotropic hardening rule expects at least the following
material properties:

- `R0`: the coefficient of the power law
- `n`: the exponent of the power law

The `p0` material property is *optional* and generally is considered a
numerical parameter to avoir an initial infinite derivative of the power
law when the exponent is lower than \(1\).

#### Example

The following code can be added in a block defining an inelastic flow:

~~~~{.cpp}
    isotropic_hardening : "Linear" {R0 : 50e6},
    isotropic_hardening : "Power" {R0 : 120e6, p0 : 1e-8, n : 5.e-2}
~~~~

# New features of the `python` bindings

## Improvements of the `tfel.math` module

### `NumPy` support

This version is the first to use `Boost/NumPy` to provide interaction
with `NumPy` array.

> **Note**
>
> The `NumPy` support is enabled by default if the `Python` bindings
> are requested. However, beware that `Boost/NumPy` is broken for
> `Python3` up to version 1.68. We strongly recommend disabling
> `NumPy` support when using previous versions of `Boost` by
> passing the flag `-Denable-numpy-support=OFF` to the `cmake`
> invokation.

### Exposing acceleration algorithm

The `FAnderson` and `UAnderson` acceleration algorithms are now
available in the `tfel.math` module. This requires `NumPy` support.

#### Example of the usage of the `UAnderson` acceleration algorithm

The following code computes the solution of the equation
\(x=\cos\paren{x}\) using a fixed point algorithm.

~~~~{.python}
from math import cos
import numpy as np
import tfel.math

# The accelerator will be based on
# the 5 last Picard iterations and
# will be performed at every step
a = tfel.math.UAnderson(5,1)

f = lambda x: np.cos(x)
    
x0 = float(10)
x = np.array([x0])    

# This shall be called each time a
# new resolution starts
a.initialize(x)
for i in range(0,100):
    x = f(x)
    print(i, x, f(x[0]))
    if(abs(f(x[0])-x[0])<1.e-14):
        break
    # apply the acceleration
    a.accelerate(x)
~~~~

Without acceleration, this algorithm takes \(78\) iterations. In
comparison, the accelerated algorithm takes \(9\) iterations.

# Improvements to the build system

## Exporting `CMake`' targets

## Disabling `NumPy` support

# Tickets solved during the development of this version

## Ticket #219: Add the possibility to define the tangent operator blocks

For more details, see: <https://sourceforge.net/p/tfel/tickets/219/>

## Ticket #212: Better `const` correctness in the `generic` behaviour

The state at the beginning of the time step is now described in a
structure called `mfront::gb::InitialState`, the fields of which are all
`const`.

The following fields of the `mfront::gb::State` are now `const`:

- `gradients`
- `material_properties`
- `external_state_variables`

For more details, see: <https://sourceforge.net/p/tfel/tickets/212/>

## Ticket #209: lack of documentation of the `@GasEquationOfState` keyword

This feature is now described in the (`MTest` page)[mtest.html]

For more details, see: <https://sourceforge.net/p/tfel/tickets/209/>

## Ticket #206: Export `TFEL` targets for use by external projects

Here is a minimal example on how to use this feature:

~~~~{.cmake}
project("tfel-test")
cmake_minimum_required(VERSION 3.0)

find_package(TFELException REQUIRED)
find_package(TFELMath REQUIRED)

add_executable(test-test test.cxx)
target_link_libraries(test-test tfel::TFELMath)
~~~~

For more details, see: <https://sourceforge.net/p/tfel/tickets/209/>

## Ticket #205: Add power isotropic hardening rule

This feature is described in Section
@sec:power_isotropic_hardening_rule.

For more details, see: <https://sourceforge.net/p/tfel/tickets/205/>

## Ticket #195 : Export variables bounds for material properties

Variables bounds (both @Bounds and @PhysicalBounds) are now available for material properties. 
They are available directly in the .so file via getExternalLibraryManager().

For more details, see: <https://sourceforge.net/p/tfel/tickets/195/>

## Ticket #202: DSLs for isotropic behaviours don't handle `NaN`

The `IsotropicMisesCreep`, `IsotropicMisesPlasticFlow`,
`IsotropicStrainHardeningMisesCreep` and
`MultipleIsotropicMisesFlowsDSL` now handle properly `NaN` values.

For more details, see: <https://sourceforge.net/p/tfel/tickets/202/>

## Ticket #196 : Export variable names for all interface of material property

The variable name of material property was available only for castem interface. 
Now it is available for all interface (c++, python, …). 
The name can be found in the .so file via getExternalLibraryManager().

For more details, see: <https://sourceforge.net/p/tfel/tickets/196/>

# References
