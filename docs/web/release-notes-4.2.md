---
title: Release notes of the 4.2 version of `TFEL`, `MFront` and `MTest`
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

# `TFEL/Math` improvements

## Workspace of nonlinear solvers {#sec:tfel_4.2:tfel_math:nonlinear_solvers:workspace}

The nonlinear solvers available in `TFEL/Math` now have an additional
optional template argument which allows the user to control how the data
members of those solvers are defined. Those data members generally
include the residual (named `fzeros`), the jacobian matrix (named
`jacobian`), etc...

The main rationale behing this development is to better handle how those
data members are allocated. By default, those data members are allocated
on the stack. This strategy is well suited on CPUs but can lead to
excessive usage of registers on GPUs.

### Example of usage

The following workspace defines the data members used by the
`TinyNewtonRaphsonSolverClass` as views into a pre-allocated buffer:

~~~~{.cxx}
template<unsigned short N, typename NumericType>
struct ExternallyAllocatedWorkspace{
  ExternallyAllocatedWorkspace(NumericType* const v)
      : fzeros(v), zeros(v + N), delta_zeros(v + 2 * N), jacobian(v + 3 * N){};
  //! \brief residual vector
  tfel::math::View<tfel::math::tvector<N, NumericType>> fzeros;
  //! \brief current estimate of the unknowns
  tfel::math::View<tfel::math::tvector<N, NumericType>> zeros;
  //! \brief current correction
  tfel::math::View<tfel::math::tvector<N, NumericType>> delta_zeros;
  //! \brief jacobian matrix
  tfel::math::View<tfel::math::tmatrix<N, N, NumericType>> jacobian;
};
~~~~

> **Note**
>
> The use is responsible for properly allocating memory.

# `MFront` improvements

## The `validator` DSL option {#sec:tfel_4.2:mfront:validator}

The `validator` option allows to specify an project or an institute
which assumes assurance quality aspects related to an `MFront` file.

The `validator` is defined as a DSL option for proper support in the
[`MFrontGallery`
project](https://thelfer.github.io/MFrontGallery/web/index.html).

### Example of usage {.unnumbered}

The following example declares the validator on the command line:

~~~~{.bash}
$ mfront --obuild --interface=generic --dsl-option='validator:BGE Tech' Plasticity.mfront
~~~~

The validator can also be defined in the the `MFront` file:

~~~~{.cxx}
@DSL Implicit{
  validator: "BGE Tech"
}
~~~~

## Allow to specify a directory where to save the `mtest` files in case of integration failure {#sec:tfel_4.2:mfront:MTEST_FILE_GENERATOR_OUTPUT_DIRECTORY}

By default, `mtest` files are generated in the current working
directory. This can be inconvenient, in particular for the users of the
`code_aster` solver, as this solver creates a temporary directory to
perform the calculations and erases this directory at the end of the
execution.

The `MTEST_FILE_GENERATOR_OUTPUT_DIRECTORY` environment variable
specifies where to put the generated `mtest` files.

# Issues fixed

## Issue 447: [mfront] Generation of MTest file shall be protected by a try/catch

For more details, see <https://github.com/thelfer/tfel/issues/447>.

## Issue 446: [mfront] Allow to specify a validator

This feature is described in Section @sec:tfel_4.2:mfront:validator.

For more details, see <https://github.com/thelfer/tfel/issues/446>.

## Issue 445: [mfront] Allow to specify a directory where to save the `mtest` files in case of integration failure

This feature is described in Section
@sec:tfel_4.2:mfront:MTEST_FILE_GENERATOR_OUTPUT_DIRECTORY.

For more details, see <https://github.com/thelfer/tfel/issues/445>.

## Issue 436: [tfel-math] Add support for external workspace in non linear solvers

This feature is described in depth in Section
@sec:tfel_4.2:tfel_math:nonlinear_solvers:workspace.

For more details, see <https://github.com/thelfer/tfel/issues/436>.

## Issue 428: [mfront] Add the methods `getMaterialKnowledgeIdentifier` and `getMaterial` to the `OverridableImplementation` class

The `getMaterialKnowledgeIdentifier` methods returns the name given to
the overridable implementation. Depending on the case, this can be the
name of the material property, the name of the behaviour or the name of
the model.

The `getMaterial` method returns the name of the material associated
with the overridable implementation.

For more details, see <https://github.com/thelfer/tfel/issues/428>.

## Issue 435: Issue with `std::tie` with `nvcc`

For more details, see <https://github.com/thelfer/tfel/issues/435>.

## Issue 427: [mtest] Add support for integral tests based on the inner pressure or the axial force in the ptest scheme

When the evolution of the inner radius or the evolution of the outer
radius is prescribed, integral tests on the inner pressure
(InnerPressure`) can be set.

When the evolution of the axial growth is prescribed, integral tests on
the axial force (`AxialForce`) can be set.

For more details, see <https://github.com/thelfer/tfel/issues/427>.

## Issue 426: [mtest] Add support for tests based on analytical formulae in the ptest scheme mtest

The `@Test` keyword now supports the `function` option, the syntax of
which is similar to the `@Test` keyword of the `mtest` scheme.

For example, the following line compares the value of the inner pressure
to the values of an evolution named `InnerPressureReferenceValues`:

~~~~{.cxx}
@Test<function> 'InnerPressure' 'InnerPressureReferenceValues' 1e2;
~~~~

For more details, see <https://github.com/thelfer/tfel/issues/426>.

## Issue 425: [mfront] query the `mdnx` path where an `OverridableImplementation` is written

The `getDestinationPathInMadnexFile` returns the `mdnx` path where an
`OverridableImplementation` is written.

For more details, see <https://github.com/thelfer/tfel/issues/425>.

## Issue 424: [madnex] Add an option to use a template file when generating a `mdnx` file

For more details, see <https://github.com/thelfer/tfel/issues/424>.

## Issue 422: Document the product of two symmetric tensors and its derivatives

For more details, see <https://github.com/thelfer/tfel/issues/422>

## Issue 383: [python-bindings] Add the ability to retrieve the `TFEL` version

For more details, see <https://github.com/thelfer/tfel/issues/383>.

## Issue 369: [TFEL/Utilities] support for raw string literals in `CxxTokenizer`

For more details, see <https://github.com/thelfer/tfel/issues/369>.

## Issue 362: [mfront] Better code generation for point-wise models using the Cast3M interface 

For more details, see <https://github.com/thelfer/tfel/issues/362>

## Issue 360: [mtest] Documentation of the  `@Profile` keyword in `PTest`

For more details, see <https://github.com/thelfer/tfel/issues/360>.

## Issue 356:  [mfront] modification of the Gurson-Tvergaard-Needleman stress criterion

For more details, see <https://github.com/thelfer/tfel/issues/356>.

## Issue 353: [mfront] Add support for unsymmetric tensorial variables when generating `MTest` files

For more details, see <https://github.com/thelfer/tfel/issues/353>.
