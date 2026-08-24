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

## Support for the `TDLS` (`TinyDeviceCallableLinearSolver`)

The `TDLS` library provides tiny linear solvers for small dense systems,
callable on `CPU`s as well as inside `GPU` kernels, and designed for
maximum GPU performance:

- <https://github.com/trsxvz/tdls>
- [API reference](https://trsxvz.github.io/TDLS/api/index.html)

When compiling `TFEL`, one must explicitly enable support for the `TDLS`
library, see below.

# Known incompatibilities

- `MFront`' interfaces have to implement a method named `setOptions`.

# Improvements to the build system

## New `cmake` options

- `enable-tdls`: add support for Tiny Device-callable Linear Solvers
  (`TDLS`) library. `OFF` by default.
- `enable-tdls-as-default-linear-system-solver-in-mfront`: use `TDLS`
  as the default linear solver. Requires `enable-tdls` to be `ON`.
  `OFF` by default.

## User-defined `cmake` variables

- `generic-parallel-configurations`: list of configurations to be
  supported. See next paragraph for details and the list of supported
  configurations.
- `TFEL_GXX_COMPILER`: path to the `g++` compiler.
- `TFEL_CLANGXX_COMPILER`: path to the `clang++` compiler.
- `TFEL_ICPX_COMPILER`: path to INTEL's `icpx` compiler.
- `TFEL_NVHPC_COMPILER`: path to NVIDIA's `nvhpc` compiler.
- `TFEL_ACPP_COMPILER`: path to AdaptativeCpp's `acpp` compiler.
- `TDLS_DIR`: directory where to find the `cmake` files installed by the
  Tiny Device-callable Linear Solvers (`TDLS`) library.

## Supported parallel configurations

The following configurations are supported for parallel computations:

- `cuda-nvcc`: configuration associated with the `CUDA` parallel
  programming model and NVIDIA's `nvcc` compiler
- `cuda-clang`: configuration associated with the `CUDA` parallel
  programming model and LLVM's `clang` compiler
- `hip-hipcc`:  configuration associated with the `HIP` parallel
  programming model and AMD's `hipcc` compiler
- `hip-clang`:  configuration associated with the `HIP` parallel
  programming model and LLVM's `clang` compiler
- `stlpar-parunseq-gcc`: configuration associated with `C++` standard
  parallel algorithms using the parallel unsequenced execution policy
  and the `gcc` compiler.
- `stlpar-parunseq-clang`: configuration associated with `C++` standard
  parallel algorithms using the parallel unsequenced execution policy
  and the `clang` compiler.
- `stlpar-parunseq-icpx`: configuration associated with `C++` standard
  parallel algorithms using the parallel unsequenced execution policy
  and the `icpx` compiler.
- `stlpar-parunseq-nvhpc-gpu`: configuration associated with `C++`
  standard parallel algorithms using the parallel unsequenced execution
  policy and NVIDIA's `nvhpc` compiler with offloading on the GPU.
- `sycl-default-icpx`: configuration associated with `SYCL` programming
  model INTEL's `icpx` compiler.

## Add a `build-tests` target

In previous versions, tests were compiled and run using the `check`
target, which internally calls `ctest` for running the test. One
drawback of this approach is that `ctest` could not be run in parallel.

The `build-tests` target (which is a dependency of the `check` target)
only compiles the tests. To run the tests, one may still call the
`check` target or call directly `ctest`.

If the `build-tests` target has not been called, the `check` target
works as in previous versions.

# New `TFEL/Config` features

The `TFEL/Config` library is now documented on [this
page](tfel-config.html).

## TFEL's default substitutions

The `getDefaultSubstitutions` function returns some default
substitutions used by `MFront` when reading configuration files and
`tfel-check` when analysing `.check` files.

### General substitutions

- `TFEL_CXX_STANDARD`: the `C++` standard used by the project.

### Supported compilers

- `TFEL_GXX_COMPILER`: path the `g++` compiler. The value associated
  with this key is determined as follows, by order of priority:

  1. by passing the variable `TFEL_GXX_COMPILER` during the invocation of
  `cmake`.
  2. by using `g++` as the main compiler to compile `TFEL`
  3. by searching `g++` using `cmake`'s `find_program` function.
- `TFEL_CLANGXX_COMPILER`: path to `LLVM` `clang++` compiler. The value
  associated with this key is determined as follows, by order of
  priority:

  1. by passing the variable `TFEL_CLANGXX_COMPILER` during the invocation of
  `cmake`,
  2. by using `clang++` as the main compiler to compile `TFEL`,
  3. by searching `clang++` using `cmake`'s `find_program` function.
- `TFEL_ICPX_COMPILER`: path to INTEL's `icpx` compiler. The value
  associated with this key is determined as follows, by order of
  priority:

  1. by passing the variable `TFEL_ICPX_COMPILER` during the invocation of
  `cmake`,
  2. by using `icpx` as the main compiler to compile `TFEL`,
  3. by searching `icpx` using `cmake`'s `find_program` function.
- `TFEL_ACPP_COMPILER`: path to AdaptativeCpp `acpp` compiler. The value
  associated with this key is determined as follows, by order of
  priority:

  1. by passing the variable `TFEL_ACPP_COMPILER` during the invocation of
  `cmake`,
  2. by searching `acpp` using `cmake`'s `find_program` function.
- `TFEL_NVHPC_COMPILER`: path to NVIDIA's `nvhpc` compiler. The value
  associated with this key is determined as follows, by order of
  priority:

  1. by passing the variable `TFEL_NVHPC_COMPILER` during the invocation of
  `cmake`,
  2. by using `nvhpc` as the main compiler to compile `TFEL`,
  3. by searching `nvhpc` using `cmake`'s `find_program` function.
- `TFEL_NVCC_COMPILER`: path to NVIDIA's `nvhcp` as found by
  `FindCUDAToolkit`.
- `TFEL_HIPCC_COMPILER`: path to AMD's `hipcc` compiler. The value
  associated with this key is determined as follows, by order of
  priority:

  1. by passing the variable `TFEL_HIPCC_COMPILER` during the invocation
    of `cmake`,
  2. by searching `hipcc` using `cmake`'s `find_program` function.

### Substitutions related to the `HIPCC` compiler

- `TFEL_HIPCC_FLAGS: additional flags to be passed to the `hipcc`
  compiler.
- `TFEL_HIP_INCLUDE_DIR`: paths to `HIP` headers

### Substitutions related to the `CUDA` platform

- `TFEL_CUDA_PATH`: path to NVIDIA's `CUDA` toolkit as found by
  `CMake`'s `FindCUDAToolkit` module.
- `TFEL_CUDA_LIBRARY_DIR`: path to the libraries of the NVIDIA's `CUDA`
  toolkit as found by `CMake`'s `FindCUDAToolkit` module.

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

## Refactoring of quantities

Quantities were deeply refactored to:

- improve maintainability (code is much more compact),
- allow creating surrogate classes for quantities,
- improve error message,
- reduce size of the generated symbols.

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

## Defining environment variables in configuration files

Configuration files can now contain an `environment_variables` sections
which can introduce a map defining a set of environment variables.

### Example of usage

~~~~{.cxx}
environment_variables: {"CXXFLAGS": "-Wall"};
~~~~

### Caution

- An environment variable can only be defined once in configuration
  files
- If an environment variable is defined in a configuration file and in a
  test file (through the `@Environment` keyword), the latter definition
  is used.

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

## Configuration file

`MFront` now allows to define options in a configuration file define
through the `--configuration-file` command line argument as follows:

~~~~{bash}
$ mfront --configuration-file=config.json --interface=generic-parallel UO2_ShearModulus.mfront
~~~~

Configuration file follows an extented `JSON`-like syntax. It contains
the following sections:

- `dsl_options`
- `material_property_dsl_options`
- `behaviour_dsl_options`
- `model_dsl_options`
- `interfaces_options`
- `compilation_options`

None of those sections is required. All those sections must introduce a
dictionary.

> **Note**
>
> The options given in sections `dsl_options`,
> `material_property_dsl_options`, `behaviour_dsl_options` and
> `model_dsl_options` may also be retrieved in dedicated configurations
> files, respectively specified by the `--dsl-options-file`,
> `--material-property-dsl-options-file`, `--behaviour-dsl-options-file`,
> and `--model-dsl-options-file` command line arguments introduced in
> Version 4.1.

### Example of configuration file

~~~~{.json}
interfaces_options: {
  generic-parallel: {
    backend: {"stlpar", execution_policy: "par_unseq"}
  }
}
~~~~

### The `interfaces_options` section

### The `compilation_options` section

~~~~{.json}
compilation_options : {
  cxx : {
    compiler: "nvc++",
    compilation_flags: "-O2 -stdpar=gpu -std=c++20 -march=native -gpu=sm_89" 
  }
}
~~~~

#### The `compiler` option

#### The `compilation_flags` option

### The `linking_options` section

#### The `linker_flags` option

## Passing options to interfaces on the command line

### First syntax

~~~~{bash}
$ mfront --obuild --interface='generic-parallel{backend:{"stlpar": {execution_policy:"par_unseq"}}}' UO2_ShearModulus.mfront
~~~~

### Second syntax

~~~~{bash}
$ mfront --obuild --interface='{generic-parallel:{backend:"stlpar"{execution_policy:"par_unseq"}}}' UO2_ShearModulus.mfront
~~~~

## The `generic-parallel` interface for material properties

See [this page for
details](generic-parallel-material-property-interface.html).

### Available backends

#### `STLPar`

#### `CUDA`

## Improvements to the `Cast3M` interface

### Improved generation of `Gibiane` input file

The `Cast3M` interface generates automatically an example of `Gibiane`
input file to integrate which each `MFront` behaviour. This example is
meant to be used a template and do not pretend to be usable without
modification by the user. For instance, previous versions of `MFront`
did not guarantee that the name of the variables (limited to `4`
characters) were unique. Version 5.2 improves this. To illustrate this,
one may compare the output of Version 5.1 and Version 5.2:

~~~~
* Version prior to 5.2
statev = 'MOTS' 'EERR' 'EEZZ' 'EETT' 'EERZ' 'KHRR' 'KHZZ' 'KHTT' 'KHRZ' 'KHRR' 'KHZZ' 'KHTT' 'KHRZ' 'P';
* Version 5.2
statev = 'MOTS' 'EERR' 'EEZZ' 'EETT' 'EERZ' 'KHRR' 'KHZZ' 'KHTT' 'KHRZ' 'V0' 'V1' 'V2' 'V3' 'P';
~~~~

In Version 5.1, the variable name `KHRR` is declared twice. `Cast3M`
does not generate an error although duplicated names leads to erroneous
results. Version 5.2 avoids this pitfall.

# Continuous integration

## Github actions

Github actions have been updated to supported a wider range of
configurations, including testing based on `Visual Studio 2024` and
`Visual Studio 2026`.

# Acknowledgements

The authors are grateful to the many contributors to the `TFEL/MFront`
project. This research was conducted in the framework of the PLEIADES
project, which was supported financially by the CEA (Commissariat à
l’Énergie Atomique et aux Énergies Alternatives), EDF (Électricité de
France) and Framatome. Homogenization developments were conducted within
the framework of the AnoHonA ANR project (n° AAPG2023).

# Issues fixed

## Issue 1016: [mfront] Variables have the same name in the `dgibi` file generated by `MFront`, for using the FEM program `Cast3M`

For more details, see <https://github.com/thelfer/tfel/issues/1016>

## Issue 1014: [cmake] Add a `build-tests` target
 
For more details, see <https://github.com/thelfer/tfel/issues/1014>

## Issue 1013: [tfel-math] Refine `QuantityConcept` to take mutability into account

For more details, see <https://github.com/thelfer/tfel/issues/1013>
￼

## Issue 1010: [clang-tidy] fix warnings related to the google-explicit-constructor check

For more details, see <https://github.com/thelfer/tfel/issues/1010>

￼
## Issue 1007: [github-actions] Add continuous integration for the generic-parallel interface with CUDA backend

For more details, see <https://github.com/thelfer/tfel/issues/1007>

## Issue 1006: [github-actions] Add tests based on `tfel-check`

For more details, see <https://github.com/thelfer/tfel/issues/1006>

## Issue 1004:  [github-actions] Add support actions based on Visual Studio

For more details, see <https://github.com/thelfer/tfel/issues/1004>

￼
## Issue 1003:[tfel-math] Improve error messages when using quantities

For more details, see <https://github.com/thelfer/tfel/issues/1003>

## Issue 1001: [tfel-math] Introduce the `QuantityConcept` class to allow surrogate classes to be treated as quantities

For more details, see <https://github.com/thelfer/tfel/issues/1001>

## Issue 995: [tfel-check] Adding @Environment section in configuration file

For more details, see <https://github.com/thelfer/tfel/issues/995>

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
