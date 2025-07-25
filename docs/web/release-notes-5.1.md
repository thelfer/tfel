---
title: Release notes of the 5.1 version of `TFEL`, `MFront` and `MTest`
author: Thomas Helfer, Antoine Martin
date: 2025
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

\newcommand{\tenseurq}[1]{\underline{\underline{\mathbf{#1}}}}
\newcommand{\paren}[1]{{\left(#1\right)}}
\newcommand{\tenseur}[1]{\underline{#1}}
\newcommand{\tepsilonto}{\tenseur{\varepsilon}^{\mathrm{to}}}
\newcommand{\tsigma}{\underline{\sigma}}
\newcommand{\trace}[1]{{\mathrm{tr}\paren{#1}}}
\newcommand{\bts}[1]{{\left.#1\right|_{t}}}
\newcommand{\mts}[1]{{\left.#1\right|_{t+\theta\,\Delta\,t}}}
\newcommand{\ets}[1]{{\left.#1\right|_{t+\Delta\,t}}}

# Known incompatibilities

- The native `Europlexus` interface has been removed, as `Europlexus`
  now uses the `generic` interface through `MGIS` (see
  <https://github.com/thelfer/tfel/issues/739> for details).
- When compiling with option `TFEL_APPEND_VERSION` set to `ON` or when
  defining the string variable `TFEL_VERSION_FLAVOUR`, the `python`
  modules are now modified to reflect those information. This old
  behaviour can be restored by setting the
  `unversioned-python-module-names` option to `ON`.
- The definition of some unit class has (`Stress`, `Temperature`, etc..)
  have been moved from `tfel::math` to `tfel::math::unit`.

## Internal API changes

`IsotropicHardeningRule::computeElasticLimitAndDerivative` now returns
the derivative of the elastic limit (computed at the middle of the time
step) with respect to the equivalent plastic strain at the middle of the
time step rather than the derivative with respect to the increment of
the equivalent plastic strain.

# New features in the `TFEL` libraries

## Environment

This version now supports a new environment variable to specify the
`TFEL` installation directory that always supersedes the `TFELHOME`
environment variable. The name of this variable depends on the version
of the project, the fact that a development version has been compiled
and the flavour (optionally specified with the `cmake` variable
`TFEL_VERSION_FLAVOUR`). The name of this variable can be retrieved with
the `--tfel-home` option of `tfel-config` described below.

## `tfel-config` {#sec:tfel_5.1:tfel_config}

### `--registry-key` option

On `Windows`, the `--registry-key` option returns the registry key which
is used to determine the TFEL's installation path.

### `--tfel-home` option

The `--tfel-home` option returns the environment variable in which the
TFEL's installation path shall be defined. 

#### Example of usage

~~~~
$ tfel-config-5.1.0-dev-release --tfel-home
TFELHOME_5_1_0_dev_release
~~~~

### `--python-module-suffix` option

This option return the suffix of the `python` module. Such suffix is not
empty only the following conditions are met:

- `TFEL` has been compiled with option `TFEL_APPEND_VERSION` set to `ON` and or
  with the string variable `TFEL_VERSION_FLAVOUR` defined,
- the `unversioned-python-module-names` option has been set to `ON` (the
  default value if `OFF`).

The option `--python-module-suffix` is only available if the python
bindings is available.

### `--config`

This option requests flags associated with the `TFELConfig` library.

### `--mfront-log-stream`

This option requests flags associated with the `MFrontLogStream` library.

### `--mfront`

This option requests flags associated with the `TFELMFront` library.

### `--mtest`

This option requests flags associated with the `TFELMTest` library.

#### Example of usage

~~~~
$ tfel-config-5.1.0-release --python-module-suffix
5_1_0_release
~~~~

# New `TFEL/Math` features

## The `tfel::types` namespace

The `tfel::types` namespace contains type aliases that are helpful to
deduce a type from another. This is mostly useful to support quantities.

The implementation of the `computeLambda` provides a simple example of
its usage:

~~~~
template <tfel::math::ScalarConcept StressType>
TFEL_HOST_DEVICE constexpr StressType computeLambda(
    const StressType& young, const types::real<StressType>& nu) noexcept
    requires(tfel::math::checkUnitCompatibility<tfel::math::unit::Stress,
                                                StressType>()) {
  return nu * young / ((1 + nu) * (1 - 2 * nu));
}
~~~~

## A `zero` method to create tensorial objects {#sec:tfel_5_1:zero}

A static method named `zero` is now available to create fixed-size
tensorial objects.

### Example of usage

~~~~{.cxx}
constexpr auto s = stensor<2u, double>::zero();
~~~~

# New `TFEL/Material` features

## Homogenization

### Ellipsoidal inclusion embedded in anisotropic matrix

When \(\tenseurq{C}_0\) is anisotropic, the Eshelby tensor can be
computed with `computeAnisotropicEshelbyTensor` in 3D and
`computePlainStrainAnisotropicEshelbyTensor` in 2D. There are also
`computeAnisotropicHillTensor`,
`computePlainStrainAnisotropicHillTensor`, and also
`computeAnisotropicLocalisationTensor` and
`computePlainStrainAnisotropicLocalisationTensor`.

### Homogenization bounds

Different homogenization bounds are implemented.
The available functions are `computeVoigtStiffness`, `computeReussStiffness`,
`computeIsotropicHashinShtrikmanBounds`.

### Homogenization schemes

Ponte Castaneda and Willis scheme for distributions
of ellipsoidal inclusions for biphasic media is now available.

## Isotropic Moduli

`IsotropicModuli` objects are defined
for the elastic moduli of an isotropic material. It makes
the manipulation of isotropic materials easier.

## Python bindings

Python bindings are now generated using the
[`pybind11`](https://github.com/pybind/pybind11) library.

The module `tfel.material` now contains the isotropic elastic moduli objects and
their functions to convert these moduli.

Moreover, some new functionalities are available for homogenization,
via the `tfel.material.homogenization` module.
It contains functions concerning:

- Hill tensors
- Localisation tensors
- Homogenization schemes for biphasic materials (Dilute scheme, Mori-Tanaka scheme,
Ponte Castaneda and Willis scheme)
- Homogenization bounds for biphasic materials (Voigt/Reuss, Hashin-Shtrikman)

# New features in `MFront`

This version introduces the following main features in `MFront`:

- warnings on  potential misuses or known bad practices,
- the ability to call other behaviour from a behaviour.

## Warnings

Many warnings have been added to detect potential misuses of `MFront` or
known bad practices.

### Activating and desactivating warnings

Reporting warnings is activated by default and can be disabled by
passing the command line argument `--report-warnings=false` to `MFront`.

Warnings are associated with keywords and code blocks. Warnings can be
disabled by appending the `safe` option to them. For instance, in an
implicit DSL, specifying a convergence threshold greater than
\(10^{-10}\) is considered to loose in most cases and thus triggers a
warning. This warning can be disabled as follows:

~~~~{.cxx}
@Epsilon<safe> 1;
~~~~

This `safe` option can be ignored by passing the `--ignore-safe` command
line argument to `MFront`. This argument is useful when analysing an
existing file (written by another person) to question implementation
choices.

### Warnings added to all DSLs

- check that the consistent tangentor operator of all the relevant
  blocks are used in `@TangentOperator`.

### Warnings added to the DSLs of the `Default` family

- check that the increments of all gradients are used in the
  `@Integrator` code block.
- check that the all thermodynamic forces are used in the
  `@Integrator` code block.
- check that the increment of all state variables are used in the
  `@Integrator` code block.
- check that all auxiliary state variables are used in the `@Integrator`
  code block or in `UpdateAuxiliaryStateVariable` code block.
- check that the consistent tangent operator `Dt` or all the tangent
  operator blocks are used in the `@PredictionOperator` code block, if
  defined.
- check that the consistent tangent operator `Dt` or all the tangent
  operator blocks are used in the `@TangentOperator` code block, if
  defined.
- check that the consistent tangent operator `Dt` or all the tangent
  operator blocks are used in the `@Integrator` code block if the
  implementation declares that the `@Integrator` computes it (using any
  of the `@ProvidesTangentOperator` and
  `@ProvidesSymmetricTangentOperator`) keywords).
- check that neither the consistent tangent operator `Dt` and any of the
  tangent operator blocks are used in the `@Integrator` code block if a
  `@TangentOperator` code block has been declared or if the
  implementation has not stated that it shall compute the consistent
  tangent operator.

### Warnings added to isotropic DSLs

#### Warnings related to the convergence threshold

- using the default value of the convergence threshold
- specifying a value of the convergence threshold too low
- specifying a value of the convergence threshold too high

#### Warnings related to the implementation of the flow rule(s)

A warning is reported if the implementation of the flow rule(s)
contains:

- the time increment `dt`,
- the strain `eto` at the beginning of the time step,
- the strain increment `deto`,
- the stress `sig`,
- the `theta` parameter,
- the increment of the external state variable,
- an auxiliary state variable not computed by an external point-wise
  model (see the `@Model` keyword).

> **Note**
>
> Using the increment of the external state variable and the
> time increment to compute the rate of an external state variable can be
> legitimate, but it is better to compute this rate in @InitLocalVariables
> and take into account the fact that the `Cast3M` solver may
> set the time increment to zero when activating is forced convergence
> algorithm.

A warning is reported if the implementation of the flow rule(s) does not
contain:

- `f` and its derivative `df_dseq`,
- `df_dp` when required,
- `R` and `dR_dp` is an isotropic harderning rule has been defined for
  this flow rule.

#### Warnings related to the convergence threshold

- using the default value of the convergence threshold
- specifying a value of the convergence threshold too low
- specifying a value of the convergence threshold too high

### Warnings added to implicit DSLs

#### Warnings related to the convergence threshold

- using the default value of the convergence threshold
- specifying a value of the convergence threshold too low
- specifying a value of the convergence threshold too high

#### Warnings related to the computation of the thermodynamic forces

- using the \(\theta\) (i.e. `theta`) or the `iterMax` parameter.
- using the time increment \(\Delta\, t\).
- using the increment of a gradient, integration variable or external
  state variable, or the increment of an auxiliary state variable
  computed by a point-wise model (see the `@Model` keyword).
- using an auxiliary state variable which is not computed by a point-wise model.

#### Warnings to `@InitializeLocalVariables`

- using the increment of an integration variable.

### Warnings added to interfaces

#### Warnings added to the `Cast3M` interface

- using a maximum number of substepping greater than 3.
- not restricting the following keywords to the `Cast3M` interface,
  which is a portability issue as the behaviour can't be compiled with
  other interfaces: `@CastemGenerateMTestFileOnFailure`,
  `@UMATGenerateMTestFileOnFailure`, `@CastemUseTimeSubStepping`,
  `@UMATUseTimeSubStepping`, `@CastemMaximumSubStepping`,
  `@UMATMaximumSubStepping`, `@CastemDoSubSteppingOnInvalidResults`,
  `@UMATDoSubSteppingOnInvalidResults`, `@CastemFiniteStrainStrategy`,
  `@UMATFiniteStrainStrategy`, `@CastemFiniteStrainStrategies` and
  `@UMATFiniteStrainStrategies`.

#### Warnings added to the `generic` interface

- not restricting the keyword
  `@GenericInterfaceGenerateMTestFileOnFailure` to the `generic`
  interface, which is a portability issue as the behaviour can't be
  compiled with other interfaces.

#### Warnings related to the perturbation value used to compute a numerical approximation of the jacobian

#### Warnings related to the convergence threshold

- using the default value
- specifying a value too low
- specifying a value too high

## Calling an external behaviour: the `@BehaviourVariable` keyword

~~~~{.cpp}
@BehaviourVariable first_phase_plastic_behaviour {
  file: "Plasticity.mfront",
  variables_suffix: "1",
  external_names_prefix: "FirstPhase",
  store_gradients: true,
  store_thermodynamic_forces: true,
  shared_material_properties: {".+"},
  shared_external_state_variables: {".+"}
};
~~~~

### Behaviour variable factories

### Extension of the `@Model` keyword

In previous versions, the `@Model` keyword allowed to call from a
behaviour point-wise models using the historical `Model` DSL.

The `@Model` keyword now allows to use point-wise models implemented
using the following DSLs: `DefaultModel`, `RungeKuttaModel` and
`ImplicitModel`. In this case, a behaviour variable factory is
automatically associated with the point-wise models which shares all its
material properties and external state variables with the calling
behaviour. Every persistent variables of the point-wise model are
declared as auxiliary state variables. For each persistent variable of
the point-wise model, a local variable meant to contain the increment of
this variable over the time step is declared.

Point-wise models are called at the initialization stage of the
behaviour. The auxiliary state variables associated with the point-wise
models are updated at beginning of the `updateAuxiliarySateVariables`
method **before** any user defined code (see the
`@UpdateAuxiliaryStateVariables` keyword).

## Uniform syntax

## New command line arguments

The following command line arguments are now supported:

- `--report-warnings`: this command line argument enables or disables
  the reporting of warnings:
  - `--report-warnings` or `--report-warnings=true` enable the reporting
    of warnings (which is the default behaviour of `MFront`).
  - `--report-warnings=false` disables the reporting of warnings.
- `--warning-error`: this command line argument allows to treat warnings
  as errors.
  - `--warning-error` or `--warning-error=true` turns warnings into
    errors.
  - `--warning-error=false` does not turn warnings into errors (which is
    the default behaviour of `MFront`).
- `-Werror` is equivalent to `--warning-error=true`.
- `-ignore-safe` allows to ignore the `safe` option of keywords and code
  blocks.

## New features in isotropic DSLs

### Stress update algorithm

Isotropic DSLs all introduced the elastic strain as a state variable.
When the elastic material properties are now to be constant in time,
then the Hooke law can be written in an incremental form
\[
\ets{\tsigma}=\bts{\tsigma}+\lambda\,\trace{\Delta\,\tepsilonto}+2\,\mu\,\paren{\Delta\,\tepsilonto-\Delta\,p\,\mts{n}}
\]
where \(\lambda\) and \(\mu\) are the first Lamé's coefficient and the
shear modulus, \(\Delta\,p\) is the increment of the equivalent plastic
strain and \(\mts{n}\) the flow direction at the middle of the time
step.

The `use_stress_update_algorithm` DSL option prevents the declaration of
the elastic strain and switches to this incremental form to compute the
stress.

#### Example of usage

~~~~{.cxx}
@DSL IsotropicStrainHardeningMisesCreep{
  use_stress_update_algorithm : true
};
~~~~

### Predefined isotropic hardening rules

The `@IsotropicHardeningRule` and `@IsotropicHardeningRules` allow to
use the isotropic hardening rules available in the
[`StandardElastoViscoPlasticity`
brick](StandardElastoViscoPlasticityBrick.html).

#### Example of usage

~~~~{.cxx}
@IsotropicHardeningRule "Voce" {flow_id : 0, R0 : 125e6, Rinf : 500e6, b : 20};
~~~~

~~~~{.cxx}
@IsotropicHardeningRules{
  flow_id : 0,
  isotropic_hardening : "Voce" {R0 : 125e6, Rinf : 500e6, b : 20},
  isotropic_hardening : "Linear" {R0 : 50e6}
};
~~~~

#### Automatic declaration of the flow rule in the `IsotropicPlasticMisesFlow` DSL

If an isotropic hardening rule is defined in the
`IsotropicPlasticMisesFlow` DSL, and if no flow rule is defined, the
following flow rule is automatically defined:

~~~~{.cxx}
@FlowRule {
  f = seq - R;
  df_dseq = 1;
  df_dp = -dR_dp;
}
~~~~

#### Available isotropic hardening rules

The following flow rules are currently available: `Data`, `Linear`,
`Power`, `StrainRateSensitive`, `Swift`, `UserDefined`, and `Voce`.

This list can be retrieved as follows:

~~~~{.bash}
$ mfront --list-isotropic-hardening-rules
~~~~

### Add support for the `@Predictor` code block

The code block `@Predictor` allows to specify an intial guess for the
(visco-)plastic strain increments.

#### Example of usage

~~~~{.cxx}
@Predictor{
 dp = sqrt(deto| deto);
}
~~~~

## Extensions of the `StandardElastoViscoPlasticity` brick

### Changing the external name of the equivalent strain {#sec:tfel:5.1:StandardElastoViscoPlasticity:equivalent_strain_external_name}

All inelastic flows now allows to change the external name of the
equilvaent strain with the `equivalent_strain_external_name` option.

#### Example of usage

~~~~{.cxx}
@Brick StandardElastoViscoPlasticity{
  stress_potential : Hooke{young_modulus : 150e9, poisson_ratio : 0.3},
  inelastic_flow : "Plastic" {
    criterion : "Mises",
    isotropic_hardening : "Linear" {R0 : 33e6, H : 438e6},
    equivalent_strain_external_name : "CumulatedEquivalentPlasticStrain"
  }
};
~~~~

### Saving the stress criterion  {#sec:tfel:5.1:StandardElastoViscoPlasticity:saving_stress_criterion}

All inelastic flows now allows the save the value of the stress
criterion at \(t+\theta\,\Delta\,t\) in a dedicated auxiliary state
variable by setting the `save_stress_criterion` option to `true`.

The external name of this auxiliary state variable defaults to
`EquivalentStress` + id, where id is the identifier of the inelastic
flow. This name can be changed using the
`stress_criterion_external_name` option.

#### Example of usage

~~~~{.cxx}
@Brick StandardElastoViscoPlasticity{
  stress_potential : Hooke{young_modulus : 150e9, poisson_ratio : 0.3},
  inelastic_flow : "Plastic" {
    criterion : "Mises",
    isotropic_hardening : "Linear" {R0 : 33e6, H : 438e6},
    stress_criterion_external_name : "StressCriterion",
    save_stress_criterion : true
  }
};
~~~~

### Saving yield radius {#sec:tfel:5.1:StandardElastoViscoPlasticity:saving_yield_surface_radius}

All inelastic flows now allows the save the value of the stress
criterion at \(t+\theta\,\Delta\,t\) in a dedicated auxiliary state
variable by setting the `save_yield_surface_radius` option to `true`.

The external name of this auxiliary state variable defaults to
`YieldSurfaceRadius` + id, where id is the identifier of the inelastic
flow. This name can be changed using the
`yield_surface_radius_external_name` option.

#### Example of usage

~~~~{.cxx}
@Brick StandardElastoViscoPlasticity{
  stress_potential : Hooke{young_modulus : 150e9, poisson_ratio : 0.3},
  inelastic_flow : "Plastic" {
    criterion : "Mises",
    isotropic_hardening : "Linear" {R0 : 33e6, H : 438e6},
    yield_surface_radius_external_name : "CurrentYieldStrength",
    save_yield_surface_radius : true
  }
};
~~~~

### Saving the thermal expansion {#sec:tfel:5.1:StandardElastoViscoPlasticity:saving_thermal_expansion}

The `save_thermal_expansion boolean` option has been introduced in all
stress potentials deriving from `the HookeStressPotentialBase`.

This option states if the computed thermal expansion(s) at the end of
the time step shall be stored in an auxiliary state variable. The
external name of this variable is `ComputedThermalExpansion`. For an
isotropic thermal expansion, this variable is a scalar. For an
orthotropic material, this variable is an array of (3) scalars.

#### Example of usage

~~~~{.cpp}
@Brick StandardElastoViscoPlasticity{
  stress_potential : Hooke{
    young_modulus : 150e9,
    poisson_ratio : 0.3,
    thermal_expansion : 1e-5,
    save_thermal_expansion : true
  }
};
~~~~

## Uniform syntaxes

### Uniform syntax for `@ElasticMaterialProperties` and `@ComputeStiffnessTensor`

The `@ElasticMaterialProperties` and `@ComputeStiffnessTensor` now
accept the same options than the stress potentials deriving from the
`HookeStressPotentialBase`:

~~~~{.cpp}
@ElasticMaterialProperties{
  young_modulus: 150e9,
  poisson_ratio: 0.3
};
~~~~

### Uniform syntax for `@ComputeThermalExpansion`

The `@ComputeThermalExpansion` keyword now accepts the same options than
the stress potentials deriving from the `HookeStressPotentialBase`:

~~~~{.cpp}
@ComputeThermalExpansion{
  thermal_expansion1: 1.e-5,
  thermal_expansion2: 0.2e-5,
  thermal_expansion3: 1.2e-5,
  thermal_expansion_reference_temperature: 293.15, 
  initial_geometry_reference_temperature: 293.15,
  save_thermal_expansion: true
};
~~~~

### Uniform syntax for `@HillTensor`

The `@HillTensor` keyword now accepts the same options than the Hill's
stress criterion available in the `StandardElastoViscoPlasticity` brick:

~~~~{.cpp}
@HillTensor H {F: 0.371, G: 0.629, H: 4.052, L: 1.5, M: 1.5, N: 1.5};
~~~~


# New features in `mfront-query`

## New command line arguments

The following command line arguments are now supported:

- `--report-warnings`: this command line argument enables or disables
  the reporting of warnings:
  - `--report-warnings` or `--report-warnings=true` enable the reporting
    of warnings (which is the default behaviour of `mfront-query`).
  - `--report-warnings=false` disables the reporting of warnings.
- `--warning-error`: this command line argument allows to treat warnings
  as errors.
  - `--warning-error` or `--warning-error=true` turns warnings into
    errors.
  - `--warning-error=false` does not turn warnings into errors (which is
    the default behaviour of `mfront-query`).
- `-Werror` is equivalent to `--warning-error=true`.

# New features in `tfel-check`

## Discard commands failure

By default, `tfel-check` discards a command failure if at least one test
is defined. The rationale behind this choice is that some command may
succeed in producing the expected results but may still fail to exit
properly.

This behavior can be controlled by the `--discard-commands-failure`
which takes `true` or `false` as argument.

### Example of usage

~~~~
$ tfel-check --discard-commands-failure=false
entering directory '/tmp/tests'
* beginning of test './test.check'
** Exec-1 ./main                                                       [ FAILED]
** Compare-1 'results.res' and 'results.res', column '1'               [SUCCESS]
* end of test './test.check'                                           [ FAILED]
======
$ tfel-check --discard-commands-failure=true
entering directory '/tmp/tests'
* beginning of test './test.check'
** Exec-1 ./main                                                       [ FAILED]
** Compare-1 'results.res' and 'results.res', column '1'               [SUCCESS]
* end of test './test.check'                                           [SUCCESS]
====== 
~~~~

## New keywords

### `@TFELLibraries`

The `@TFELLibraries` keyword let the user specify TFEL libraries to link
with. This keyword must be followed by an array of strings.

The following libraries are available: `Config`, `Exception`,
`Glossary`, `Tests`, `UnicodeSupport`, `Utilities`, `System`, `Math`,
`MathCubicSpline`, `MathKriging`, `MathParser`, `NUMODIS`, `Material`,
`MFront`, `MTest`.

#### Example of usage

~~~~{.cpp}
@TFELLibraries {"MathParser"};
~~~~

## New tutorials

New tutorials on implementation of homogenization schemes
for biphasic linear elastic media are available in the gallery.

New tutorials on implementation of homogenization schemes
in non-linear elasticity are available in the gallery: Taylor scheme,
Sachs scheme, $\beta$-rule. These tutorials use the 
keyword `@BehaviourVariable` for the integration and the computation
of tangent operator of the local behaviours. The implementation
shows how to use any behaviour law on each phase.

# Python bindings

Python bindings are now generated using the
[`pybind11`](https://github.com/pybind/pybind11) library.

## New features in the `mfront` module

- The `setDebugMode` function is now available.

# Issues fixed

## Issue 822: [calculix-interface] add info about SDV variables in the generated example of input file

The list of internal state variables are now shown in the comment of the
generated example of input file:

~~~~
** 
** File generated by MFront from the Norton.mfront source
** Example of how to use the Norton behaviour law
** Author Helfer Thomas
** Date   23 / 11 / 06
**

*Material, name=@CALCULIXBEHAVIOUR_NORTON
** list of internal state variables:
** <SDV1: ElasticStrain_0>, <SDV2: ElasticStrain_1>, <SDV3: ElasticStrain_2>, 
** <SDV4: ElasticStrain_3>, <SDV5: ElasticStrain_4>, <SDV6: ElasticStrain_5>,
** <SDV7: EquivalentViscoplasticStrain>
*Depvar
7
** The material properties are given as if we used parameters to explicitly
** display their names. Users shall replace those declaration by
** theirs values
*User Material, constants=4
<NortonCoefficient>, <NortonExponent>, <YoungModulus>, <PoissonRatio>

~~~~

For more details, see <https://github.com/thelfer/tfel/issues/822>

## Issue 793: add @Predictor support for isotropic DSLs

For more details, see <https://github.com/thelfer/tfel/issues/793>

## Issue 790: [tfel-math] Remove `Stress`, `Time` from the `tfel::math` namespace

For more details, see <https://github.com/thelfer/tfel/issues/790>

## Issue 789: [mfront] advice to remove include and src in case of compilation error

For more details, see <https://github.com/thelfer/tfel/issues/789>

## Issue 779: [tfel-check] Failure during parsing step if a symbolic linked was not correctly generated

For more details, see <https://github.com/thelfer/tfel/issues/779>.

## Issue 778: [tfel-check] Add an option to return a failure if at least one check fails

For more details, see <https://github.com/thelfer/tfel/issues/778>

## Issue 777: [tfel-check] Clearer message if a directory is given as an argument

For more details, see <https://github.com/thelfer/tfel/issues/777>

## Issue 763: Personalize names of equivalent plastic strain in `StandardElastoViscoPlasticity`'s inelastic_flow

This feature is described in Section
@sec:tfel:5.1:StandardElastoViscoPlasticity:equivalent_strain_external_name.

For more details, see <https://github.com/thelfer/tfel/issues/763>

## Issue 762: Save the isotropic_hardening's threshold stress via an auxiliary state variable using `StandardElastoViscoPlasticity`'s inelastic_flow

This feature is described in Section
@sec:tfel:5.1:StandardElastoViscoPlasticity:saving_yield_surface_radius.

For more details, see <https://github.com/thelfer/tfel/issues/762>

## Issue 761: Save the equivalent stress via an `@AuxiliaryStateVariable` using `StandardElastoViscoPlasticity`'s inelastic_flow

This feature is described in Section
@sec:tfel:5.1:StandardElastoViscoPlasticity:saving_stress_criterion.

For more details, see <https://github.com/thelfer/tfel/issues/761>

## Issue 760: Save the thermal expansion tensor via an `@AuxiliaryStateVariable` using `StandardElastoViscoPlasticity`'s stress_potential 

This feature is described in Section
@sec:tfel:5.1:StandardElastoViscoPlasticity:saving_thermal_expansion.

For more details, see <https://github.com/thelfer/tfel/issues/760>

## Issue 739: [mfront] Depreciation of europlexus interface

For more details, see <https://github.com/thelfer/tfel/issues/739>

## Issue 708: Add support for hardening modes when `libc++` is used

For more details, see <https://github.com/thelfer/tfel/issues/708>

## Issue 724: [mfront] Add support for non constant elastic properties in the stress update algorithm of the isotropic DSLs

For more details, see <https://github.com/thelfer/tfel/issues/724>

## Issue 721: [tfel-math] Add a `zero` method to create tensorial objects

This feature is described in Section @sec:tfel_5_1:zero.

For more details, see <https://github.com/thelfer/tfel/issues/721>

## Issue 717: [mfront] Add warning if the increment of a state variable is not used in `@Integrator` for the `Implicit` DSLs and the `Default` DSLs
￼

For more details, see <https://github.com/thelfer/tfel/issues/717>

## Issue 698: [mfront] Add a stress update option which would not use the elastic strain as a state variable in Isotropic DSLs

For more details, see <https://github.com/thelfer/tfel/issues/698>

## Issue 661: [mfront] Allow to use isotropic hardening rules in isotropic DSLs

For more details, see <https://github.com/thelfer/tfel/issues/661>

## Issue 702:  [mfront] Add a warning if an auxiliary state variable not computed by an external point-wise model is used in a `@FlowRule` block in isotropic DSLs

For more details, see <https://github.com/thelfer/tfel/issues/702>

￼
## Issue #697: [cyrano] Add a warning when the maximum number of sub steppings is too high

For more details, see <https://github.com/thelfer/tfel/issues/697>

## Issue #696: [calculix-interface] add support for the `@GenerateMTestFileOnFailure` keyword

For more details, see <https://github.com/thelfer/tfel/issues/696>

## Issue #695: [aster-interface] add support for the `@GenerateMTestFileOnFailure` keyword

For more details, see <https://github.com/thelfer/tfel/issues/695>

## Issue #694: [ansys-interface] add support for the `@GenerateMTestFileOnFailure` keyword

For more details, see <https://github.com/thelfer/tfel/issues/694>

## Issue #692: [mfront] add warning when interface specific keywords are not restricted to the corresponding interface

For more details, see <https://github.com/thelfer/tfel/issues/692>

## Issue #690: [castem] Add a warning when the maximum number of sub steppings is too high

For more details, see <https://github.com/thelfer/tfel/issues/690>

## Issue #689: [implicit-dsl] Add warning when using auxiliary state variables (not computed by external point-wise models) in `@ComputeStress` and `@ComputeFinalStress`

For more details, see <https://github.com/thelfer/tfel/issues/689>

## Issue 685: [tfel-config] Add support for `TFELConfig`, `TFELMFront` and `TFELMTest`

This feature is described in Section @sec:tfel_5.1:tfel_config.

For more details, see <https://github.com/thelfer/tfel/issues/685>

## Issue 684: [mfront] Add support for `@Includes`, `@Link` and `@TFELLibraries` to the `Model\ DSL

For more details, see <https://github.com/thelfer/tfel/issues/684>

## Issue 683: Using `TFEL` librairies in material properties

For more details, see <https://github.com/thelfer/tfel/issues/683>

## Issue 677:[tfel-config] add a python-module-suffix option

For more details, see <https://github.com/thelfer/tfel/issues/677>

## Issue 676: Always prefer the versioned `TFELHOME` environment variable

For more details, see <https://github.com/thelfer/tfel/issues/676>

## Issue 674: [python-bindings] Add option to disable the modication of the names of  python modules by the `TFEL_APPEND_VERSION` and `TFEL_VERSION_FLAVOUR`

For more details, see <https://github.com/thelfer/tfel/issues/674>

## Issue 673: [tfel-config] Add the ability to define an environment variable associated with the `TFEL`'s version and flavour

For more details, see <https://github.com/thelfer/tfel/issues/673>

## Issue 672: [python bindings] fix python modules names when `TFEL_APPEND_VERSION` or `TFEL_VERSION_FLAVOUR` is defined

For more details, see <https://github.com/thelfer/tfel/issues/672>

## Issue 667: Reduce the size of files paths in sources to fit in `Windows`'s `MAX_PATH` limit

For more details, see <https://github.com/thelfer/tfel/issues/667>

## Issue 666: [doc] Add CONTRIBUTING file, remove devel.md

For more details, see <https://github.com/thelfer/tfel/issues/666>

## Issue 654: Add PowerShell environment script

For more details, see <https://github.com/thelfer/tfel/issues/654>

## Issue 640: [mfront] Only point-wise models defined by `Model` DSL can be embedded in behaviours using the `@Model` keyword 

For more details, see <https://github.com/thelfer/tfel/issues/640>

## Issue 294: [python-bindings] Evaluate port to `pybind`

For more details, see <https://github.com/thelfer/tfel/issues/293>

