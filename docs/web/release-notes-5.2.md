---
title: Release notes of the 5.2 version of `TFEL`, `MFront` and `MTest`
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


The page describes the new functionalities of Version 5.2 of the
`TFEL` project.

# Highlights

## Support for the `TDLS` (`TinyDeviceCalledLinearSolver`)

The `TDLS` library provides an highly optimized linear solver on `CPU`s
and `GPU`s:

- <https://github.com/trsxvz/tdls>
- [API reference](https://trsxvz.github.io/TDLS/api/index.html)


# New `TFEL/Math` features

## Views of tensorial objects with coalescent memory access for data separated by a constant stride

In previous versions, the `View` class allows interpreting a contiguous
memory area as a tensorial object. The `CoalescedView` class allows to
create a tensorial object where each component is referred to by its own
pointer.

The `StridedCoalescedView` assumes that each component is separated by a
constant stride in memory, so that it only have to store one pointer and
the value of the stride.

Such views are efficient on GPUs where coalescent memory access are
preferable.

~~~~{.cxx}
int values[8] = {1, 10, 2, 20, 3, 30, 4, 40};
auto s1 = map_strided<stensor<2u, int>>(values, 2);
~~~~

## Walpole basis

The Walpole basis relative to transverse isotropic fourth-order tensors (`st2tost2`)
is introduced with the `struct` `TransverseIsotropicWalpoleBasis`, member of
`tfel::math`.

# New `TFEL/Material` features

## Homogenization

The following methods of the ParticulateMicrostructure objects are able
to modify the microstructure:
 
 - `changeElasticityOfMatrixPhase`
 - `changeElasticityOfInclusionPhase`
 - `changeFractionOfInclusionPhase`

The `UserDefinedDistributionOfSpheroids` allows to define
a distribution of spheroids defined by two orientation
tensors. This distribution can be used in a `ParticulateMicrostructure`
and the associated homogenization schemes can be used with
these microstructures.

# New features in `tfel-check`

## Parallelization

The commands described in different `.check` files can now be run in
distinct processes.

Note that the command described in a `.check` are still run
sequentially.

By default, as in previous versions, only one process is used.

### Specifying the number of jobs run simultaneously

The command line argument `--jobs` (`-j`) allow specifying the number
of jobs run simultaneously.

If no option is given to `--jobs`, the number of available cores
is used, if the system provide this information.

#### Limit on the number of jobs

By default, this number must be lower than the number of cores
available. This limit can be discarded by
passing`--discard-jobs-limit=true` to `tfel-check`.

### Caution

The user must take care that the commands described in `.check` files
can be run in parallel.

- the jobs shall no exhaust memory.
- the jobs shall not access concurrently to the same resources.

A typical example of conflict would be to have two `.check` files in the
same directory that call `mfront` to build somes shared libraries: the
two `MFront`'s processes may write concurrently files in the `src` and
`include` directories, leading to an undefined behaviour.

Another example would be to use `@CleanDirectories` and `@CleanFiles`
commands inappropriately and erase data used by commands of other
`.check` files.

##  New features in `TFEL/System`

### Improvements in `ProcessManager`

- New overloads of the `createProcess` and `execute` methods allow
  specifying the directory where a command shall be executed.
- `ProcessManager` shall now work properly in a multithreaded context.

### Improvements in `SignalManager`

- `SignalManager` shall now work properly in a multithreaded context.

# New `TFEL/Material` features

## Homogenization

The second moments of strains considering a Hashin-Shtrikman scheme
are available. A new [tutorial](PonteCastaneda1992.html)
shows how to use it in the implementation of Ponte-Castaneda variational
bound (1992), within the framework of non-linear elasticity.

# New features in `MFront`

## The `@LinearSystemSolver` keyword

The `@LinearSystemSolver` keyword is used to select the linear sytem
solver. It is followed by the name of solver. Currently, two solvers
are supported:

- `Default`. This uses the solvers provided natively by the `TFEL/Math`
  library. As implied by its name, this solver is used by default.
- `TDLS` (or equivalently `TinyDeviceCallableLinearSolver`). See
  <https://trsxvz.github.io/TDLS/getting_started.html> for details. If
  `TFEL` is not compiled with support of the `TDLS` library selecting
  this solver falls back the the `Default` Solver.

The name of the solver can optionnaly be followed by a dictionary
allowing to parametrize the solver.

### Supported Parameters

#### Parameters of the `Default` Linear Solver

The `Default` Linear Solver does not accept any parameter.

#### Parameters of the `TDLS` Linear Solver

The following options are supported:

- `tile_size`: the matrix is processed as a grid of `tile_size x
  tile_size` register tiles. This is the main performance axis of the
  solvers - tune it per system dimension. The `tile_size` can be a
  strictly positive integer or a string. In `tile_size` is a string, it
  is interpreted as a integer formula whose variable can be any integer
  constant (see the `@IntegerConstant` keyword), `N`, `TVectorSize`,
  `StensorSize` or `TensorSize`. This formula must evaluate to a
  strictly positive integer for all hypotheses supported by the
  behaviour.
- `schedule`: expected values are either `left_looking` (or equivalently
  `left`, `left-looking` or `LeftLooking`) and `right_looking` (or
  equivalently `right`, `right-looking` or `RightLooking`). This
  parameter defines the elimination schedule of the tiled factorization.
- `out_of_tile_search_strategy`: expected values are either
  `first_acceptable` (or equivalently `first-acceptable` or
  `FirstAcceptable`) or `full_scan` (or equivalently `full-scan` or
  `FullScan`).
- unroll_inner_loop: expected values are either `true` or `false`. If
  `true`, loops indexing register tiles carry a forced-unroll pragma -
  the guard that keeps tiles in registers on GPU backends, where a
  rolled loop indexes the tile dynamically and demotes it to slow local
  memory. If `false`, no unroll pragma anywhere - faster compiles, GPU
  performance not guaranteed.

If a parameter is not defined, the default values are inherited for the
`TiledLUppDefaultConfig` class defined by the `TDLS` library. See the
[API reference](https://trsxvz.github.io/TDLS/api/index.html) for
details. At the time of writing this documentation, those default values
are:

- `3` for `tile_size`
- `right_looking` for `schedule`
- `first_acceptable` for `out_of_tile_search_strategy`
- `true` for `unroll_inner_loop`

### Examples of usage

~~~~ {#LinearSystemSolver .cpp}
@LinearSystemSolver TDLS{
  tile_size: 4,
  unroll_inner_loop: false
};
~~~~

~~~~ {#LinearSystemSolver .cpp}
@LinearSystemSolver TDLS{
  tile_size: "StensorSize"
};
~~~~

# Acknowledgements

The authors are grateful to the many contributors to the `TFEL/MFront`
project. This research was conducted in the framework of the PLEIADES
project, which was supported financially by the CEA (Commissariat à
l’Énergie Atomique et aux Énergies Alternatives), EDF (Électricité de
France) and Framatome. Homogenization developments were conducted within
the framework of the AnoHonA ANR project (n° AAPG2023).

# Issues fixed

## Issue 987: `StridedCoalescedView` has no exposed data pointer and stride

For more details, see <https://github.com/thelfer/tfel/issues/987>

## Issue 926: [tfel-math] Add `TFEL_HOST_DEVICE` to `LeftCauchyGreenTensorDerivativeExpr` and `RightCauchyGreenTensorDerivativeExpr` classes

For more details, see <https://github.com/thelfer/tfel/issues/926>

## Issue 925: [tfel-math] Introduce the `StressStensorConcept` and `NoUnitStensorConcept` concepts and siblings for orther tensorial objects

For more details, see <https://github.com/thelfer/tfel/issues/925>

## Issue 922: [tfel-math] two functions have no GPU device support
￼

For more details, see <https://github.com/thelfer/tfel/issues/922>

## Issue 890: [mfront][homogenization] Add Ponte Castaneda 1992 non linear scheme

For more details, see <https://github.com/thelfer/tfel/issues/890>

## Issue 905: [TFEL/Math] Pointer-heavy CoalescedView

For more details, see <https://github.com/thelfer/tfel/issues/905>

## Issue 853: [tfel-fcheck] Have the possibility to launch multiple call to `tfel-check` simultaenously in a CI context

For more details, see <https://github.com/thelfer/tfel/issues/853>
