---
title: General documentation
author: Thomas Helfer, Jean-Michel Proix
date: 17/08/2021
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

Welcome to the **official documentation** for TFEL and MFront. This
section provides a **structured overview** of all available resources,
organized by topic and difficulty level.

This page provides several links to available documentations regarding
`TFEL` and `MFront`, which are fairly detailed. Unfortunately, most of
these materials are written in French. They also mostly refer to
versions `2.0.x`, and thus do not describe the latest available version.

For a new (English speaking) user, we would highly recommend:

- Read the **reference paper** @helfer_introducing_2015. Although this
  paper covers versions `2.0.x`, it gives a good general overview of
  `TFEL` and `MFront`'s abilities.
- Read the [`getting-started` page](getting-started.html).
- Read the page dedicated to second and fourth order tensors,
  [here](tensors.html).
- Have a look at the [**gallery**](gallery.html) which details the
  implementations of various classical mechanical behaviours. Those
  implementations are kept up-to-date to use the latest developments
  and techniques introduced in `TFEL` and `MFront`.
- If none of the implementations described in the gallery match the user
  need, one may have a look at the implementations provided in the
  `mfront/tests/behaviours` directory in the sources. Beware that those
  implementations are not updated on purpose to ensure backward
  compatibility. Most of them can be simplified by using
  [bricks](BehaviourBricks.html).
- Finally, you may want to have a look at the documentation of the
  interface of the solver that you plan to use (see below for links).

For Spanish users, Jordi Alberich has written "A Spanish introduction
to `MFront` and `Salome-MECA`", which is available
[here](tutorial-spanish.html).

# MFront documentation

## References

- General introduction. Material properties and Models: [mfront.pdf](documents/mfront/mfront.pdf)
- Writing mechanical behaviours: [behaviours.pdf](documents/mfront/behaviours.pdf)
- How to handle plane stress in implicit schemes: [planestress.pdf](documents/mfront/planestress.pdf)
- Finite strain behaviours [finitestrain.pdf](documents/mfront/finitestrain.pdf)
- `MTest` user guide: [mtest.pdf](documents/mtest/mtest.pdf)

## 🛠 Tutorials & Examples

Learn by doing with hands-on examples.

| Resource | Description | Language | Difficulty |
|----------|-------------|----------|------------|
| [MFront Gallery](gallery.html) | Step-by-step implementations of classical behaviours. | English | ⭐⭐ |
| [Tutorial (French)](tutorial.html) | Complete tutorial for beginners (Norton law example). | French | ⭐⭐ |
| [Spanish Tutorial](tutorial-spanish.html) | Introduction to MFront and Salome-MECA. | Spanish | ⭐⭐ |
| [Behaviour Examples](https://github.com/thelfer/tfel/tree/master/mfront/tests/behaviours) | Official test cases (GitHub). | Code | ⭐⭐⭐ |

## Courses

- General introduction: [mfront-presentation.pdf](documents/mfront-presentation/mfront-presentation.pdf)
- Writing mechanical behaviours [mfront-behaviours.pdf](documents/mfront-behaviours/mfront-behaviours.pdf)
- General introduction [mfront-formation.pdf](documents/mfront-formation/mfront-formation.pdf)
    - [tp.pdf](documents/tp/tp.pdf)

## Talks

- Finite strain: [mfront-finitestrain.pdf](documents/mfront-finitestrain/mfront-finitestrain.pdf)
- [Writing portable behaviours](documents/portable-behaviour/portability.html)

A `github` repository has been set up to store various documents
describing TFEL and MFront usage. The talks of the first and second
MFront Users are available there:

<https://github.com/thelfer/tfel-doc>

## Material Knowledge Types

| Resource | Description |
|----------|-------------|
| [Material Properties](material-properties.html) | How to define material properties (e.g., Young's modulus, thermal conductivity). |
| [Mechanical Behaviours](behaviours.html) | In-depth guide to implementing mechanical behaviours. |
| [Models](models.html) | Simple point-wise models (e.g., swelling for fuel performance codes). |

## Domain specific languages

| Resource | Description |
|----------|-------------|
| [Implicit](implicit-dsl.html) | Implicit integration for strain-based behaviours. |

## Bricks and Extensions

| Resource | Description |
|----------|-------------|
| [Behaviour Bricks](BehaviourBricks.html)          | Pre-defined bricks to simplify behaviour implementation. |
| [`StandardElastoViscoplasticity` brick](StandardElastoViscoPlasticityBrick.html) | Brick allowing to implement a vast class of plastic and viscoplastic behaviours |
| [`StandardElasticity`](HookeStressPotential.html) | Brick used by behaviours based on an elastic part described by Hooke's law|
| [`FiniteStrainSingleCrystal`](finitestrainsinglecrystal.html) | Brick dedicated to finite strain behaviour describing single crystals |
| [`DDIF2` brick](DDIF2Brick.html)                  | Brick based the `DDIF2` damage law used in various fuel performance code developed at CEA |

## Homogenization

| Resource | Description |
|----------|-------------|
| [Sachs Scheme](Sachs.html) | Lower bound for polycrystals. |
| [Taylor Scheme](Taylor.html) | Upper bound for polycrystals. |
| [Ponte Castaneda](PonteCastaneda1992.html) | Non-linear elastic composites. |
| [Masson Affine Formulation](MassonAffineFormulation.html) | Affine formulation for viscoplastic polycrystals. |
| [Biphasic Linear Homogenization](BiphasicLinearHomogenization.html) | Homogenization for biphasic media. |
| [β-Rule](BetaRule.html) | Implementation of the β-rule for homogenization. |

## Post-Processing & Outputs

| Resource | Description |
|----------|-------------|
| [Post-Processings](post-processings.html) | How to extract and analyze results. |
| [Parameters](parameters.html) | Material parameters and their usage. |

## 🔌 Interfaces

### Interfaces dedicated to material properties

| Interface| Description | Documentation |
|--------|-------------|----------------|
| **Generic interface** | For use with the [MGIS project](https://thelfer.github.io/mgis/web/index.html). | [Generic Interface]( generic-material-property-interface.md) |
| **Generic parallel interface** | For use with the [MGIS project](https://thelfer.github.io/mgis/web/index.html). | [Generic Interface](generic-parallel-material-property-interface.md) |

### Mechanical behaviours

TFEL/MFront supports multiple finite element solvers. Each interface has its own documentation.

| Interface | Description | Documentation |
|--------|-------------|----------------|
| **Abaqus** | Commercial FEA solver (Standard/Explicit). | [Abaqus Interface](abaqus.html) |
| **Code_Aster** | Open-source FEA solver developped by EDF. | [Aster Interface](aster.html) |
| **Cast3M** | CEA's finite element solver. | [Cast3M Interface](castem.html) |
| **CalculiX** | Open-source FEA solver. | [CalculiX Interface](calculix.html) |
| **ANSYS** | Commercial FEA solver (APDL). | [ANSYS Interface](ansys.html) |
| **ZeBuLoN (Zmat)** | Zset's finite element solver. | [Zmat Interface](zmat.html) |
| **Cyrano** | Fuel performance code (CEA). | [Cyrano Interface](cyrano.html) |
| **ElmerFEM** | Open-source multiphysics solver. | [ElmerFEM Interface](elmerfem.html) |
| **AMITEX_FFTP** | FFT-based solver. | [AMITEX Interface](amitex.html) |
| **Generic interface** | For use with the [MGIS project](https://thelfer.github.io/mgis/web/index.html). | [Generic Interface](generic-behaviours-interface.html) |

## 📖 Advanced topics ⭐⭐/⭐⭐⭐

| Resource | Description |
|----------|-------------|
| [Theory](theory.html) | Theoretical background for material behaviours. |
| [mfront types](mfront-types.html) | Predefined aliases in `MFront` |
| [The `@BehaviourVariable` keyword](behaviour-variable.html) | Calling behaviours inside behaviours |
| [Unicode](unicode.html) | Unicode support in `MFront` |
| [Glossary](glossary.html) | Definitions of all TFEL/MFront terms. |
| [madnex](madnex.html) | Support of `madnex` files in `MFront` |
| [mfront-query](mfront-query.html) | Command line queries about MFront files |
| [mfront-doc](mfront-doc.html) | A documentation generator from MFront files |
| [The `mfm` tool](mfm.html) | A tool for querying entry points in `MFront` generated libraries |
| [Compilation process](compiling-mfront-shared-libraries.html) | Compilation process used by `MFront`|
| [Fisher-Burmeister complementary function](FischerBurmeister.html) | On the Fisher-Burmeister complementary function and its applications to multi-surface plasticity |
| [Extending StandardElastoViscoPlasticity](ExtendingStandardElastoViscoPlasticityBrick-StressCriterion.html) | How to extend standard bricks with custom criteria. |

## Keywords associated with Domain-Specific Languages (DSL)

Note the those pages may be outdated or can not apply to your particular
installation of `TFEL/MFront`. We highly recommend using the command
line to retrieve the list of keywords associated with a `DSL` or the
documentation associated with a keyword.

| DSL | Description | Keywords Reference |
|-----|-------------|---------------------|
| Implicit | Implicit integration for strain-based behaviours. | [Implicit Keywords](Implicit-keywords.html) |
| Default | Generic DSL for custom integration algorithms. | [Default Keywords](Default-keywords.html) |
| Runge-Kutta | Explicit integration for ODE-based behaviours. | [Runge-Kutta Keywords](RungeKutta-keywords.html) |
| IsotropicPlasticMisesFlow | DSL for isotropic J₂ plasticity. | [Keywords](IsotropicPlasticMisesFlow-keywords.html) |

# 🧪 Testing & Validation ⭐⭐

Tools and guides for testing your implementations.

| Resource | Description |
|----------|-------------|
| [MTest](mtest.html) | TFEL's testing tool for material point simulations. |
| [MTest Nonlinear Constraints](mtest-nonlinear-constraints.html) | Advanced constraints in MTest. |
| [TFEL-Check](tfel-check.html) | Validation suite for TFEL. |
| [MTest Keywords](MTest-keywords.html) | Keywords reference for MTest. |
| [mfm-test-generator](mfm-test-generator.html) | Generation of integration tests for various solvers. |

# Python modules ⭐⭐

| Resource | Description |
|----------|-------------|
| [TFEL Python](tfel-python.html) | bindings for TFEL libraries. |
| [MFront Python](mfront-python.html) | Python bindings for MFront. |
| [MTest Python](mtest-python.html) | Python bindings for MTest. |

# 📊 Advanced Topics ⭐⭐/⭐⭐⭐

For experienced users.

## TFEL libraries

| Library | Description |
|----------|-------------|
| [TFEL/Math](tfel-math.html) | Overview of the TFEL/Math library. |
| [TFEL/Material](tfel-material.html) | Overview of the TFEL/Material library. |
| [TFEL/PhysicalConstants](physical-constants.html) | Pre-defined physical constants in TFEL. |
| [TFEL/MFrontDataBase](tfel-mfront-database.html) | Utilities to handle MFront generated libraries. |
| [TFEL/System](TFELSystem.html) | Overview of the TFEL/System library. |
| [TFEL/Config](tfel-config.html) | Overview of the TFEL/Config library. |

Usage of the TFEL libraries in `C++` is described in [this page](libraries_usage.html).

## Mathematical Foundations

| Resource | Description |
|----------|-------------|
| [Tensors](tensors.html) | Operations on 2nd and 4th order tensors (TFEL/Math). |
| [Numerical Integration](tfel-math-numerical-integration.html) | Numerical integration methods in TFEL. |
| [Data Interpolation](tfel-math-data-interpolation.html) | Interpolation techniques. |

## Doxygen documentation

The `Doxygen` documentation is available [here](https://thelfer.github.io/tfel/doxygen/index.html).

# 📌 Legend

- ⭐: Beginner
- ⭐⭐: Intermediate
- ⭐⭐⭐: Advanced

# Bibliography

<!-- Local IspellDict: english -->
