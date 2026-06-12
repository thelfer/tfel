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

# Known incompatibilities

- `MFront`' interfaces have to implement a method named `setOptions`.

# Improvements to the build system

## User-defined `cmake` variables

- `generic-parallel-configurations`: list of configurations to be
  supported. See next paragraph for details and the list of supported
  configurations.
- `TFEL_GXX_COMPILER`: path to the `g++` compiler.
- `TFEL_CLANGXX_COMPILER`: path to the `clang++` compiler.
- `TFEL_ICPX_COMPILER`: path to INTEL's `icpx` compiler.
- `TFEL_NVHPC_COMPILER`: path to NVIDIA's `nvhpc` compiler.
- `TFEL_ACPP_COMPILER`: path to AdaptativeCpp's `acpp` compiler.

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

# New `MFront` features

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


# Acknowledgements

The authors are grateful to the many contributors to the `TFEL/MFront`
project. This research was conducted in the framework of the PLEIADES
project, which was supported financially by the CEA (Commissariat à
l’Énergie Atomique et aux Énergies Alternatives), EDF (Électricité de
France) and Framatome. Homogenization developments were conducted within
the framework of the AnoHonA ANR project (n° AAPG2023).

# Issues fixed

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