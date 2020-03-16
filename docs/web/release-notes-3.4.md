% New functionalities of the 3.4 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2020

\newcommand{\paren}[1]{{\left(#1\right)}}

The page declares the new functionalities of the 3.4 version of the
`TFEL` project.

# New features of `MFront`

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
