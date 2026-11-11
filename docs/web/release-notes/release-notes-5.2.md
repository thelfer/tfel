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

## Support for the `TDLS` library (Tiny Device-callable Linear Solvers)

The `TDLS` library provides direct linear solvers for small dense
systems, callable on `CPU`s as well as inside `GPU` kernels, and
designed for maximum GPU performance:

- <https://github.com/trsxvz/tdls>
- [API reference](https://trsxvz.github.io/TDLS/api/index.html)

When compiling `TFEL`, one must explicitly enable support for the `TDLS`
library, see below.

## Compatibility modes

The behaviour of `MFront` and the results of the code it generates are
now governed by a compatibility mode, which is a version number. A
development which does not fix an explicit bug, but changes the results
of the `TFEL` libraries or the results or the behaviour of `MFront` and
of the assets it generates, such as a new default value of an option or
a new default behaviour of a working keyword, is associated with the
version which makes it the default. Users who need the previous
behaviour restore it by selecting an older compatibility mode.

Version 5.2 provides two compatibility modes:

- `5.2.0`, the values and behaviours of Version 5.2,
- `future`, which introduces the changes which shall be the default of
  the next major version.

By default, no compatibility mode is declared: the generated code uses
the entities and the algorithms of the version of the `TFEL` libraries
which compiles it. Declaring a compatibility mode makes the results of
the generated code independent of this version.

The compatibility mode of a file is defined by the `compatibility_mode`
option of its domain specific language, which is available for all the
DSLs, whereas the `--compatibility-mode` command line argument declares
the mode of all the files treated by a run of `mfront` which do not
declare their own mode:

~~~~{.bash}
$ mfront --obuild --interface=generic --compatibility-mode=5.2.0 Norton.mfront
~~~~

The compatibility mode of an asset which declares one is recorded in its
generated code as the `<entry point>_compatibility_mode` symbol, which is
returned by the `ExternalLibraryManager::getCompatibilityMode` method,
available both in `C++` and in `Python`.

The values of the physical constants are the first entities to be
versioned this way. The `TFEL/PhysicalConstants` library now provides
one set of constants per compatibility mode, in namespaces named after
the version which introduced them (`tfel::v5_2_0::`, `tfel::v6_0_0::`,
...), whereas the names used by the previous versions,
`tfel::PhysicalConstants` and the variables of `tfel::constants`, keep
referring to the values of the current version. The current values of
Version 5.2 are those of the `5.2.0` mode, so the physical constants of
version 5.2 are those of version 5.1: the use of the values of the
`CODATA` 2014 adjustment with all the digits of the `double`
representation is postponed to version 6.0, where it is available
through the `6.0.0` set of constants.

Compatibility modes are described in the pages devoted to
[compatibility modes in the `TFEL` libraries](tfel-compatibility-modes.html)
and to
[compatibility modes in `MFront`](mfront-compatibility-modes.html).

# Known incompatibilities

- The `enable-sanitize-options` option is no longer a boolean option: it
  now takes the list of the sanitizers to enable, e.g.
  `-Denable-sanitize-options="address;undefined"`. Enabling all the
  sanitizers at once was indeed meaningless as most sanitizers cannot be
  combined (for instance `address`, `thread` and `memory`). Each
  sanitizer can also be enabled individually using its dedicated
  `enable-<name>-sanitizer` option, see below.
- `cmake` \(3.20\) or later is now required to build `TFEL`.
- `MFront`' interfaces have to implement a method named `setOptions`.
- The boolean argument of `CxxTokenizer::isValidIdentifier` now states,
  as documented, if `C++` keywords are allowed (see Issue #964): its
  meaning has been inverted. With the default value (`true`), `C++`
  keywords are now considered as valid identifiers.
- The static method `get` of the
  `GlobalDomainSpecificLanguageOptionsManager` class has been removed.
  The global options passed to domain specific languages are now handled
  by the `ConfigurationManager` class, which derives from the
  `GlobalDomainSpecificLanguageOptionsManager` class and whose unique
  instance is returned by `ConfigurationManager::get()`.
- `@RotationMatrix<Euler>` of `MTest` now gives the rotation described
  by the Euler angles in the convention of Bunge, as the polycrystal
  classes of `MFront` (see Issue #1270): the rotation matrix was the
  Bunge matrix instead of its transpose, i.e. the inverse rotation was
  applied.
- `Evaluator::getVariablesNames` returns the names of the variables
  ordered by their positions (the order given to the constructor, or the
  order of their first appearance in the expression), instead of the
  alphabetical order (see Issue #1209).
- `MTest` (see Issue #1260): an incomplete set of orthotropic thermal
  expansion coefficients is now an error (except `ThermalExpansion1` and
  `ThermalExpansion2` in plane stress).
- `MTest` assembles the equations associated with the off-diagonal
  components of the deformation gradient of finite strain behaviours
  with the off-diagonal stresses, and applies the imposed off-diagonal
  Cauchy stresses to both of their rows (see Issue #1290): computations
  leaving one of these components free, which failed or converged to a
  wrong state, converge to the right state, and the number of
  iterations of the other ones may change.
- `abaqusexplicit` interface (see Issue #1274): the results of
  orthotropic behaviours using the `MFront` orthotropy management policy
  change when the material frame is not symmetric with respect to the
  global frame: the integration applied the inverse rotation, and the
  second material axis was read beyond the orientation in 2D.
- `computeDeterminantSecondDerivative` of a non-symmetric tensor now
  returns the second derivative of the determinant in 2D and 3D (see
  Issue #1262): the rows of the off-diagonal components differ from the
  previous results, which were the derivative of the adjugate.
- The tiny non-linear solvers (and the `Implicit` DSLs) evaluate the
  estimate given by the last allowed correction (see Issue #1288): an
  integration that converges with exactly `iterMax` corrections now
  succeeds, and a failing one evaluates its residual once more.
- The Levenberg-Marquardt minimisers (`LevenbergMarquardt`,
  `FSLevenbergMarquardt`) reject the steps that increase the cost (see
  Issue #1288): the iterates of every fit change.
- Inputs that were silently ignored are now rejected (see Issue #1256):
  the `overriding_parameters` option of the model DSLs, `@Bounds` and
  `@PhysicalBounds` on gradients and thermodynamic forces, bounds on a
  component of a scalar variable, and the `Ksf` option of the
  `HarmonicSumOfNortonHoffViscoplasticFlows` inelastic flow.
- The Broyden algorithms of the `Implicit` DSLs (`Broyden`,
  `PowellDogLeg_Broyden` and `Broyden2`) update the jacobian, or its
  inverse, again (see Issue #1295): since `TFEL-4.0`, the update was
  hidden by the `updateOrCheckJacobian` method generated in every
  behaviour. The number of iterations of the behaviours using them
  changes, and so do their results within the convergence criterion.
  In `TinyBroydenSolver` and `TinyPowellDogLegBroydenSolver`
  (respectively `TinyBroyden2Solver`), the update is performed by the
  `updateJacobian` (respectively `updateJacobianInvert`) method, which
  replaces their `updateOrCheckJacobian` method.
- The integer values read by `CxxTokenizer::readInt` and
  `CxxTokenizer::readUnsignedInt`, i.e. the integers of the `MTest`,
  `MFront` and `tfel-check` files (numbers of iterations and of
  sub-steps, columns, array sizes of the DSLs, ...), shall be written
  without decimal point nor exponent: `3.0`, `3.` or `3e0` are now
  rejected (see Issue #1326).
- The type identifiers exported by `MFront` for the variables whose
  type is the derivative of a tensorial type alias (such as
  `StressStensor`, `Stensor`, `TVector` or `Tensor`) with respect to a
  non-scalar type follow the encoding described in the documentation:
  for instance, `derivative_type<StressStensor, StrainStensor>` is
  exported as `268`, like `Stensor4`, instead of `76`, which `MTest` and
  `MGIS` decode as `derivative_type<stensor<1, real>, real>`. Behaviours
  declaring such variables must be generated again (see Issue #1332).
- The array sizes of the variables of `MFront` and the integer
  formulas evaluated by `IntegerEvaluator` (for instance the
  `tile_size` option of the `TDLS` linear solver) shall only contain
  integers: a number with a decimal point or an exponent, such as `2.5`
  (which was truncated to `2`), `1e1` or `2.0`, is now rejected (see
  Issue #1321).
- In the homogenisation schemes of `TFEL/Material`, the elasticity of an
  anisotropic `OrientedDistribution` or `Grain`, given in the local basis
  of its inclusions, is rotated to the global basis (see Issue #1299):
  it was used as a global elasticity, so that the orientation of these
  phases was ignored. `computeEllipsoidLocalisationTensor` and
  `computePlaneStrainLocalisationTensor` with an isotropic matrix rotate
  the elasticity of the inclusion as well, as documented. The results of
  the dilute, Mori-Tanaka and self-consistent schemes with such phases
  change.
- The `FiniteStrainSingleCrystal` brick takes into account, in the
  jacobian and the consistent tangent operator, the derivatives of the
  normalisation of the inverse of the plastic deformation gradient
  increment (see Issues #1309 and #1359). These derivatives were
  explicitly neglected, as numerical tests showed (or at least seemed to
  show) that they were negligible. The residual is unchanged, so the
  results only change within the convergence criteria, unless the
  sub-stepping changes: the number of local and global iterations
  changes, and decreases significantly when the deformation gradient is
  not fully imposed. The boolean option
  `plastic_deformation_gradient_normalisation_corrections` of the brick,
  which is `true` by default, allows users to easily test if this
  correction affects their results: setting it to `false` restores the
  previous derivatives.

# Improvements to the build system

## New `cmake` options

- `enable-tdls`: add support for Tiny Device-callable Linear Solvers
  (`TDLS`) library. `OFF` by default.
- `enable-tdls-as-default-linear-system-solver-in-mfront`: use `TDLS`
  as the default linear solver. Requires `enable-tdls` to be `ON`.
  `OFF` by default.
- `enable-libfuzzer-tests`: configure the `libFuzzer` based analyses of
  the `tfel::utilities::CxxTokenizer` class. `OFF` by default. Those
  analyses can only be built with `clang` and are neither built nor run
  by `make check`, see the `build-cxxtokenzier-libfuzzer-tests` and
  `cxxtokenzier-libfuzzer-tests` targets described below.

## Sanitizers and hardening options

The support for sanitizers has been modularised: the `enable-sanitize-options`
option takes the list of the sanitizers to enable (a comma may be used as a
separator instead of a semicolon), for instance:

~~~~{.bash}
$ cmake <srcdir> -Denable-sanitize-options="address;undefined"
~~~~

The available sanitizers depend on the compiler and on its version. The
`address`, `thread` and `memory` sanitizers cannot be used simultaneously.

Each sanitizer also has a dedicated option, whose name is obtained from the
sanitizer name: `enable-<name>-sanitizer`, e.g.
`-Denable-bounds-strict-sanitizer=ON`. Such options can be combined with the
`enable-sanitize-options` option: the enabled sanitizers are the sanitizers
listed in the `enable-sanitize-options` option and the sanitizers whose
dedicated option is enabled. Dedicated options are also defined for the
sanitizers which are not supported by the current compiler (enabling them
issues a warning and they are ignored), so that the same options can be
used whatever the compiler is.

The following sanitizers are declared for `gcc` (the sanitizers corresponding
to a subset of the undefined behaviour sanitizer can be enabled
individually):

- `address`, `thread`, `leak`, `hwaddress`,
- `undefined` and its subsets: `alignment`, `bool`, `bounds`,
  `bounds-strict`, `builtin`, `enum`, `float-cast-overflow`,
  `float-divide-by-zero`, `null`, `object-size`, `return`, `shift`,
  `shift-base`, `shift-exponent`, `signed-integer-overflow`, `vptr`,
- `pointer-compare`, `pointer-subtract` (the address sanitizer and the link
  time optimization are required and added automatically),
- hardening flags, which can be combined with the sanitizers above:
  `fstack-check`, `fstack-protector-all`, `fno-omit-frame-pointer`.

The following sanitizers are declared for `clang`:

- `address`, `thread`, `memory` (the memory sanitizer also enables
  `-fsanitize-memory-track-origins=2`), `leak`, `hwaddress`,
- `cfi`, `cfi-icall`, `safe-stack`, `shadow-call-stack`, `dataflow`,
- `undefined` and its subsets: `alignment`, `bool`, `bounds`, `builtin`,
  `enum`, `float-cast-overflow`, `float-divide-by-zero`, `integer`,
  `unsigned-integer-overflow`, `null`, `object-size`, `return`, `shift`,
  `shift-base`, `shift-exponent`, `signed-integer-overflow`, `vptr`,
- `pointer-compare`, `pointer-subtract` (the address sanitizer and the link
  time optimization are required and added automatically),
- hardening flags, which can be combined with the sanitizers above:
  `address-use-after-scope` (`-fsanitize-address-use-after-scope`),
  `fstack-check`, `fstack-protector-all`, `fno-omit-frame-pointer`.

Some sanitizers require specific settings to be fully effective (the link
time optimization for `cfi` and `shadow-call-stack`, instrumentation of all
the linked code for `memory` and `safe-stack`, etc.), as described in the
`gcc` and `clang` documentations.

Note that the `vptr` check, which is part of the `undefined` sanitizer,
may lead to pathologically long compilation times with `gcc` for some
files of `MFront`, which implement heavy template code: it takes more
than ninety minutes to compile
`mfront/src/GenericMaterialPropertyInterfaceBase.cxx` with
`-fsanitize=undefined` (`-O2`), whereas a few minutes are needed when the
`vptr` check is disabled. This check can be excluded by listing
individually the sanitizers of interest, using the corresponding
dedicated options (`alignment`, `bool`, `bounds`, `builtin`, `enum`,
etc.) instead of the `undefined` sanitizer.

When at least one sanitizer is enabled, the `-fno-omit-frame-pointer` flag is
added automatically to ease the interpretation of the reports emitted by the
sanitizers.

## User-defined `cmake` variables

- `generic-parallel-configurations`: list of configurations to be
  supported. See next paragraph for details and the list of supported
  configurations.
- `TFEL_GXX_COMPILER`: path to the `g++` compiler.
- `TFEL_CLANGXX_COMPILER`: path to the `clang++` compiler.
- `TFEL_ICPX_COMPILER`: path to INTEL's `icpx` compiler.
- `TFEL_NVHPC_COMPILER`: path to NVIDIA's `nvhpc` compiler.
- `TFEL_ACPP_COMPILER`: path to AdaptativeCpp's `acpp` compiler.
- `tdls_DIR`: directory where to find the `cmake` files installed by the
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
- `sycl-default-acpp`: configuration associated with `SYCL` programming
  model and AdaptativeCpp's `acpp` compiler.
- `sycl-gpu-acpp`: configuration associated with `SYCL` programming
  model and AdaptativeCpp's `acpp` compiler, with offloading on the GPU.

## Add a `build-tests` target

In previous versions, tests were compiled and run using the `check`
target, which internally calls `ctest` for running the test. One
drawback of this approach is that `ctest` could not be run in parallel.

The `build-tests` target (which is a dependency of the `check` target)
only compiles the tests. To run the tests, one may still call the
`check` target or call directly `ctest`.

If the `build-tests` target has not been called, the `check` target
works as in previous versions.

## Fuzz testing the tokenizer with `libFuzzer`

The `tfel::utilities::CxxTokenizer` class, which `MFront` uses to read
the files of its domain specific language, is now covered by coverage
guided fuzz testing, gathered in the `additional-tests/fuzzing`
directory of the sources. Those analyses are configured by the
`enable-libfuzzer-tests` option, which is `OFF` by default, and can
only be built with `clang`.

Two targets are dedicated to them:

- `build-cxxtokenzier-libfuzzer-tests` builds the fuzzing executables,
- `cxxtokenzier-libfuzzer-tests` builds them and runs the analyses, for
  the duration given by the `libfuzzer-tests-max-total-time` option.

The fuzzing executables are excluded from the default target and no
analysis is registered with `ctest`, so that neither `make all` nor
`make check` builds or runs them: the duration of a fuzzing analysis is
not bounded a priori. The corpus of an analysis and the inputs it
generates which lead to a failure are kept in the build tree, so that a
long campaign is a sequence of runs sharing the same corpus.

The `mfront-fuzzing` analysis tokenizes `MFront` files with the options
`MFront` itself uses, and checks the contract of the produced tokens
beyond the absence of any sanitizer report. The `all` value of the
`mfront-fuzzing-database` option uses all the `MFront` files of
`mfront/tests/behaviours` as the database of the analysis.

For more details, see the documentation on [fuzz testing the
tokenizer](testing-fuzzing.html).

# New `TFEL/Config` features

The `TFEL/Config` library is now documented on [this
page](tfel-config.html).

## TFEL's default substitutions

The `getTFELDefaultSubstitutions` function returns some default
substitutions used by `MFront` when reading configuration files and
`tfel-check` when analysing `.check` files. The keys listed below are
delimited by the `@` character in those files and in the map returned
by this function, e.g. `@TFEL_CXX_STANDARD@`.

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
  3. by searching `nvc++` using `cmake`'s `find_program` function.
- `TFEL_NVCC_COMPILER`: path to NVIDIA's `nvcc` as found by
  `FindCUDAToolkit`.
- `TFEL_HIPCC_COMPILER`: path to AMD's `hipcc` compiler. The value
  associated with this key is determined as follows, by order of
  priority:

  1. by passing the variable `TFEL_HIPCC_COMPILER` during the invocation
    of `cmake`,
  2. by searching `hipcc` using `cmake`'s `find_program` function.

### Substitutions related to the `HIPCC` compiler

- `TFEL_HIPCC_FLAGS`: additional flags to be passed to the `hipcc`
  compiler.
- `TFEL_HIP_INCLUDE_DIR`: paths to `HIP` headers

### Substitutions related to the `CUDA` platform

- `TFEL_CUDA_PATH`: path to NVIDIA's `CUDA` toolkit as found by
  `CMake`'s `FindCUDAToolkit` module.
- `TFEL_CUDA_LIBRARY_DIR`: path to the libraries of the NVIDIA's `CUDA`
  toolkit as found by `CMake`'s `FindCUDAToolkit` module.

## `--cmake-prefix-path`

`--cmake-prefix-path` returns the path to the `CMake` configuration
files provided by the project

~~~~{.bash}
$ tfel-config --cmake-prefix-path
/home/th202608/codes/tfel/master/install-python-3.10.12/share/tfel/cmake
~~~~

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

## Adjugate of a tensor and its derivative

The `computeAdjugateDeterminantDerivative` function returns the
adjugate of a tensor, \(\det\left(F\right)\,F^{-1}\), i.e. the
transpose of the derivative of the determinant returned by
`computeDeterminantDerivative`. The
`computeAdjugateDeterminantSecondDerivative` function returns its
derivative, which `computeDeterminantSecondDerivative` returned in 2D
and 3D before Issue #1262 (it now returns the second derivative of the
determinant).

~~~~{.cxx}
const auto adj = computeAdjugateDeterminantDerivative(F);
const auto dadj = computeAdjugateDeterminantSecondDerivative(F);
~~~~

# New `TFEL/Material` features

## Homogenization

The following methods of the `ParticulateMicrostructure` objects are able
to modify the microstructure:
 
 - `changeElasticityOfMatrixPhase`
 - `changeElasticityOfInclusionPhase`
 - `changeFractionOfInclusionPhase`

The `UserDefinedDistributionOfSpheroids` allows to define
a distribution of spheroids defined by two orientation
tensors. This distribution can be used in a `ParticulateMicrostructure`
and the associated homogenization schemes can be used with
these microstructures.

The `Polycrystal` object has been added to deal with
homogenization schemes which do not assume that there is a matrix phase.
An object `Grain` was created for this class of microstructures.

The `computeSelfConsistent` function (`computeSelfConsistentScheme` in
the `Python` bindings) has been modified in order to be
more consistent with the literature. Now, This scheme takes a `Polycrystal`
in which no matrix phase exists. For the `ParticulateMicrostructure`,
the function `computeAsymmetricSelfConsistent`
(`computeAsymmetricSelfConsistentScheme` in the `Python` bindings) has
been introduced, which corresponds to a variant of the Self-Consistent
scheme for which the matrix phase plays a particular role.

In the homogenization schemes relative to the `ParticulateMicrostructure`,
the computation of the derivatives of the homogenized stiffness tensor
w.r.t. the isotropic moduli is available. It can be obtained
by using the attributes `.derivative_of_homogenized_stiffness_wrt_kr`
and `.derivative_of_homogenized_stiffness_wrt_mur` of the
`HomogenizationScheme` object (only for isotropic phases).

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

## Controlling clean up of generated files and directories

The `--clean-up` command line argument allows to choose whether the
generated files and directories listed by the `@CleanFiles` and
`@CleanDirectories` keywords will be cleaned (this is the case by
default) or not in case of success.

~~~~{.bash}
$ tfel-check --clean-up=false
~~~~

The `--discard-clean-up-on-failure` command line argument allows to
choose whether the generated files and directories listed by the
`@CleanFiles` and `@CleanDirectories` keywords will be cleaned or not
(this is the case by default) in case of failure.

~~~~{.bash}
$ tfel-check --discard-clean-up-on-failure=false
~~~~

## Timeout for commands

Commands now have a `timeout` option to limit their duration:

~~~~{c++}
@Command "sleep 10"{
 timeout: 1
};
~~~~

This feature requires the `timeout` utility to be be available.

#  New features in `TFEL/System`

## Improvements in `ProcessManager`

- New overloads of the `createProcess` and `execute` methods allow
  specifying the directory where a command shall be executed.
- `ProcessManager` shall now work properly in a multithreaded context.

## Improvements in `SignalManager`

- `SignalManager` shall now work properly in a multithreaded context.

# New `TFEL/Material` features

## Homogenization

The second moments of strains considering a Hashin-Shtrikman scheme
are available. A new [tutorial](PonteCastaneda1992.html)
shows how to use it in the implementation of Ponte-Castaneda variational
bound (1992), within the framework of non-linear elasticity.

# New features in `MFront`

## The `@LinearSystemSolver` keyword

The `@LinearSystemSolver` keyword is used to select the linear system
solver. It is followed by the name of the solver. Currently, two solvers
are supported:

- `Default`. This uses the solvers provided natively by the `TFEL/Math`
  library. As implied by its name, this solver is used by default.
- `TDLS` (or equivalently `TinyDeviceCallableLinearSolver`). See
  <https://trsxvz.github.io/TDLS/getting_started.html> for details. If
  `TFEL` is not compiled with support for the `TDLS` library, selecting
  this solver falls back to the `Default` solver.

The name of the solver can optionally be followed by a dictionary
allowing to parametrize the solver.

### Supported Parameters

#### Parameters of the `Default` Linear Solver

The `Default` Linear Solver does not accept any parameter.

#### Parameters of the `TDLS` Linear Solver

The following options are supported:

- `tile_size`: the matrix is processed as a grid of `tile_size x
  tile_size` register tiles. This is the main performance axis of the
  solvers. Tune it per system dimension. The `tile_size` can be a
  strictly positive integer or a string. If `tile_size` is a string, it
  is interpreted as an integer formula whose variables can be any integer
  constant (see the `@IntegerConstant` keyword), `N`, `TVectorSize`,
  `StensorSize` or `TensorSize`. This formula must evaluate to a
  strictly positive integer for all hypotheses supported by the
  behaviour.
- `schedule`: expected values are either `left_looking` (or equivalently
  `left`, `left-looking` or `LeftLooking`) or `right_looking` (or
  equivalently `right`, `right-looking` or `RightLooking`). This
  parameter defines the elimination schedule of the tiled factorization.
- `out_of_tile_search_strategy`: expected values are either
  `first_acceptable` (or equivalently `first-acceptable` or
  `FirstAcceptable`) or `full_scan` (or equivalently `full-scan` or
  `FullScan`). With `first_acceptable`, the search below the tile stops
  at the first candidate whose corrected magnitude reaches the
  out-of-tile search threshold. With `full_scan`, all the rows below
  the tile are scanned and the best corrected candidate wins.
- `unroll_inner_loop`: expected values are either `true` or `false`. If
  `true`, loops indexing register tiles carry a forced-unroll pragma,
  the guard that keeps tiles in registers on GPU backends, where a
  rolled loop indexes the tile dynamically and demotes it to slow local
  memory. If `false`, no unroll pragma anywhere, for faster compiles,
  GPU performance not guaranteed.
- `out_of_tile_search_threshold`: acceptable-pivot threshold of the
  out-of-tile search. An in-tile pivot candidate whose magnitude reaches
  this value is accepted without looking outside the tile; below it, the
  search extends to the rows under the tile (out-of-tile pivoting) and
  the best candidate wins.
- `relative_pivot_threshold`: relative threshold of the pivot choice
  inside the diagonal tile, in `]0, 1]`. The row in place keeps the
  pivot when its magnitude reaches both this fraction of the largest
  in-tile candidate and the out-of-tile search threshold; otherwise the
  largest in-tile candidate wins. This threshold governs the in-tile
  choice only: the out-of-tile search keeps its absolute threshold. A
  value of `1` takes the largest in-tile candidate. This option requires
  a version of `TDLS` later than 0.2.0.
- `singular_pivot_threshold`: the factorization is declared singular
  when even the best candidate of the out-of-tile recovery stays below
  this threshold. This option must be set together with the
  `out_of_tile_search_threshold` option and must not exceed it.
- `out_of_tile_search_diagnostics`: expected values are either `true`
  or `false`. If `true`, an integer member `mfront_tdls_oot_count`,
  readable in user code blocks, reports the number of out-of-tile
  searches of the last linear solve. If `false` (the default), the
  generated code is unchanged.

If a parameter is not defined, the default values are inherited from the
`TiledLUppConfig` class defined by the `TDLS` library. See the
[API reference](https://trsxvz.github.io/TDLS/api/index.html) for
details. At the time of writing this documentation, those default values
are:

- `3` for `tile_size`
- `right_looking` for `schedule`
- `first_acceptable` for `out_of_tile_search_strategy`
- `true` for `unroll_inner_loop`
- `1e-10` for `out_of_tile_search_threshold` (or `1e-4` for single
  precision)
- `0.1` for `relative_pivot_threshold` (`TDLS` 0.2.0 takes the largest
  in-tile candidate, as a threshold of `1`)
- `std::numeric_limits<double>::min()` for `singular_pivot_threshold`
  (or `std::numeric_limits<float>::min()` for single precision)

### Examples

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
- `material_property_interfaces_options`
- `behaviour_interfaces_options`
- `model_interfaces_options`
- `compilation_options`
- `linking_options`

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
    backend: {stlpar: {execution_policy: "par_unseq"}}
  }
}
~~~~

### The `interfaces_options` section

The `interfaces_options` section associates to the name of an interface
a dictionary of options passed to this interface, as in the previous
example. The `material_property_interfaces_options`,
`behaviour_interfaces_options` and `model_interfaces_options` sections
have the same syntax, but their options are only passed to the
interfaces dedicated to material properties, behaviours and models
respectively.

### The `compilation_options` section

The `compilation_options` section associates to a language (`c`,
`cxx`, `cuda` or `hip`) a dictionary of options, as follows:

~~~~{.json}
compilation_options : {
  cxx : {
    compiler: "nvc++",
    compilation_flags: "-O2 -stdpar=gpu -std=c++20 -march=native -gpu=sm_89" 
  }
}
~~~~

#### The `compiler` option

The `compiler` option specifies the compiler used for the given
language. The environment variables `CC`, `CXX`, `CUDA_COMPILER` and
`HIP_COMPILER` take precedence over this option.

#### The `compilation_flags` option

The `compilation_flags` option specifies the compilation flags used for
the given language, as a string or as an array of strings. The
environment variables `CFLAGS`, `CXXFLAGS`, `CUDAFLAGS` and `HIPFLAGS`
take precedence over this option.

#### The `preprocessor_flags` option

The `preprocessor_flags` option specifies the preprocessor flags used
for the given language, as a string or as an array of strings. Those
flags are written before the compilation flags.

### The `linking_options` section

The `linking_options` section defines the options passed to the linker.

#### The `linker_flags` option

The `linker_flags` option specifies the flags passed to the linker, as a
string or as an array of strings. The environment variable `LDFLAGS`
takes precedence over this option.

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

## The `plastic_deformation_gradient_normalisation_corrections` option of the `FiniteStrainSingleCrystal` brick

The `FiniteStrainSingleCrystal` brick normalises the inverse of the
plastic deformation gradient increment by the cubic root of its
determinant. The derivatives of this normalisation with respect to the
plastic slips were explicitly neglected in the jacobian and in the
consistent tangent operator, as numerical tests showed (or at least
seemed to show) that they were negligible.

These derivatives are now taken into account by default (see Issue
#1359). The results are not affected, up to the convergence criteria,
but the convergence is when the deformation gradient is not fully
imposed: the global iterations recover their quadratic convergence. The
boolean option
`plastic_deformation_gradient_normalisation_corrections` allows to
neglect them:

~~~~{.cpp}
@Brick FiniteStrainSingleCrystal{
  plastic_deformation_gradient_normalisation_corrections : false
};
~~~~

The same derivatives are explicitly neglected in the
`FiniteStrainSingleCrystal`, `FiniteStrainSingleCrystal2` and
`FiniteStrainSingleCrystal_NumericalJacobian` test behaviours. Versions
taking them into account are provided by the
`FiniteStrainSingleCrystal_WithCorrections`,
`FiniteStrainSingleCrystal2_WithCorrections` and
`FiniteStrainSingleCrystal_NumericalJacobian_WithCorrections` test
behaviours.

# Documentation

## New coding guidelines page

A new page describes the coding conventions of the `C` and `C++`
sources of the project, for new developers as well as for coding
agents: [coding guidelines](coding-guidelines.html).

## New compatibility modes pages

Two new pages describe how compatibility modes are implemented in the
`TFEL` libraries ([compatibility modes in the `TFEL`
libraries](tfel-compatibility-modes.html)) and how they are used by
`MFront` ([compatibility modes in
`MFront`](mfront-compatibility-modes.html)).

# Continuous integration

## Github actions

Github actions have been updated to supported a wider range of
configurations, including testing based on `Visual Studio 2022` and
`Visual Studio 2026`.

# Acknowledgements

The authors are grateful to the many contributors to the `TFEL/MFront`
project. This research was conducted in the framework of the PLEIADES
project, which was supported financially by the CEA (Commissariat à
l’Énergie Atomique et aux Énergies Alternatives), EDF (Électricité de
France) and Framatome. Homogenization developments were conducted within
the framework of the AnoHonA ANR project (n° AAPG2023).

# Issues fixed

## Issue 1387: [TFEL/Utilities] tests `CxxTokenizer` with `libFuzzer`

The `tfel::utilities::CxxTokenizer` class is now covered by `libFuzzer`
based analyses, which are enabled by the `enable-libfuzzer-tests`
option, and by the `CxxTokenizerContractTest` unit test, which checks
the contract of the tokens for the lexical constructs that were not
covered by the existing tests of the tokenizer: raw string literals,
empty comments, comments of a line and numbers with digit separators.

For more details, see <https://github.com/thelfer/tfel/issues/1387>

## Issue 1379: [mfront] Chaboche 2012 kinematic hardening rule: are the differences with the literature for m != 1 intended?

For more details, see <https://github.com/thelfer/tfel/issues/1379>

## Issue 932: [mtest] Document modeling of pipes

For more details, see <https://github.com/thelfer/tfel/issues/932>

## Issue 1376: [mfront-doc] Formula of the Chaboche 2012 kinematic hardening rule differs from the code for m != 1

For more details, see <https://github.com/thelfer/tfel/issues/1376>
## Issue 1359: [mfront] FiniteStrainSingleCrystal: corrections of the derivatives of the normalised plastic deformation gradient increment, test behaviours and option of the brick

For more details, see <https://github.com/thelfer/tfel/issues/1359>
## Issue 1273: [tfel-check] Warning when @Precision with one value keeps the second precision of a previous @Precision

For more details, see <https://github.com/thelfer/tfel/issues/1273>
## Issue 1371: [mtest] Generalised plane strain elastic stiffness used with the generic plane stress algorithm of the castem wrappers

For more details, see <https://github.com/thelfer/tfel/issues/1371>

## Issue 1332: [mfront] type identifiers of the derivatives of tensorial type aliases (derivative_type<StressStensor, StrainStensor>) are decoded as another type by MTest and MGIS

For more details, see <https://github.com/thelfer/tfel/issues/1332>

## Issue 1330: [docs] StrainMeasure options, MTest prediction policies and keywords, implicit DSL tangent operator, tfel-check options, crystal plasticity, acceleration algorithms, cyrano generalised plane stress

For more details, see <https://github.com/thelfer/tfel/issues/1330>

## Issue 1326: [mfront] behaviour interfaces: Hencky strain in plane stress, calculix orthotropic logarithmic strain, MTest files of abaqus and ansys, cyrano NTENS, castem and abaqus examples, negative unsigned values of the DSLs

For more details, see <https://github.com/thelfer/tfel/issues/1326>

## Issue 1324: [tfel-material] GTN equivalent stress for small porosities, Norton-Hoff harmonic sum derivative, criteria with quantities, Schmid factors of cubic crystals, documentation of criteria and nucleation models

For more details, see <https://github.com/thelfer/tfel/issues/1324>

## Issue 1321: [tfel-math, tfel-system] formula parsers, raw streams, recursiveFind, ThreadPool, Python tfel.system: non-integer sizes, missing derivatives, nested external functions, ln and H in C++, pointers, short writes, symbolic links

For more details, see <https://github.com/thelfer/tfel/issues/1321>

## Issue 1319: [mfront] RungeKuttaFiniteStrain with the deformation gradient, truncated files, command-line keywords of mtest, unsigned parameters, MaterialLaw bounds, Python mfront module, empty configuration options, tfel-config of the CMake generator

For more details, see <https://github.com/thelfer/tfel/issues/1319>

## Issue 1317: [tfel-math] Include guard of runtime_array, assignment from views arrays, strided indexing, Runge-Kutta final time and null step, scalarNewtonRaphson last iterate, QRDecomp of wide matrices, Levenberg-Marquardt with one parameter, abs of vector expressions

For more details, see <https://github.com/thelfer/tfel/issues/1317>

## Issue 1313: [mtest] Prediction of finite strain behaviours, derivative types with a scalar side, plane stress messages, time step scaling of generic behaviours, UAnderson, PTest mandrel, mfm-test-generator

For more details, see <https://github.com/thelfer/tfel/issues/1313>

## Issue 1311: [mfront] Material properties given by formulas or external laws do not compile with @UseQt

For more details, see <https://github.com/thelfer/tfel/issues/1311>

## Issue 1309: [mfront] Bricks: staggered porosity scheme, ScalarDamage plane stress, DDIF2 tangent operator, FiniteStrainSingleCrystal derivatives, saved criteria, Data extrapolation, untested components

For more details, see <https://github.com/thelfer/tfel/issues/1309>

## Issue 1307: [mfront] UMAT-like interfaces: Ansys finite strain shear components, plane stress expansions, time step scaling factors, ZMAT bounds, Ansys energies, Abaqus/Explicit exceptions

For more details, see <https://github.com/thelfer/tfel/issues/1307>

## Issue 1305: [mfront] Material property and model interfaces: validator symbols, octave static parameters, castem bounds and models, gnuplot @Graph, java with MSVC, bounds of model outputs, out of bounds policy of generic models

For more details, see <https://github.com/thelfer/tfel/issues/1305>

## Issue 1303: [mfront] Implicit DSLs: inverse jacobian blocks, normalisation factors, reference temperature of external files, UserDefined and Broyden2 algorithms, reserved names

For more details, see <https://github.com/thelfer/tfel/issues/1303>

## Issue 1301: Fixes of the 5.2 review: incomplete fixes and regressions in merged code

For more details, see <https://github.com/thelfer/tfel/issues/1301>

## Issue 1299: TFEL/Material homogenisation: the stiffness of oriented anisotropic inclusions and grains, given in their local basis, is used as a global stiffness

For more details, see <https://github.com/thelfer/tfel/issues/1299>

## Issue 1295: [tfel-math] The Broyden solvers never update the jacobian in MFront behaviours

For more details, see <https://github.com/thelfer/tfel/issues/1295>

## Issue 1292: [tfel-material] LogarithmicStrainHandler: wrong tangent moduli in 3D when two eigenvalues of C are equal

For more details, see <https://github.com/thelfer/tfel/issues/1292>

## Issue 1290: [mtest] Off-diagonal deformation gradient equations and imposed off-diagonal stresses at finite strain, Broyden test behaviours overwriting the decomposed jacobian, implicitmonocrystal tests running MonoCrystal, elastic stiffness of the castem generic plane stress algorithm

For more details, see <https://github.com/thelfer/tfel/issues/1290>

## Issue 1288: [tfel-math] Adaptive Gauss-Kronrod integration with quantities, last correction of the tiny non-linear solvers, Levenberg-Marquardt steps increasing the cost, ExternalLibraryManager library path, contract violations aborting Python

For more details, see <https://github.com/thelfer/tfel/issues/1288>

## Issue 1285: [mfront] generic interface, finite strain in plane stress: the Green-Lagrange and standard finite strain families add the axial deformation gradient to the one of the caller

For more details, see <https://github.com/thelfer/tfel/issues/1285>

## Issue 1283: [mfront] MTest files generated on failure by the generic interface cannot be replayed in plane stress, and in plane strain at finite strain

For more details, see <https://github.com/thelfer/tfel/issues/1283>

## Issue 1281: [mfront] The generated Makefile adds the calling directory, not src, to LD_LIBRARY_PATH

For more details, see <https://github.com/thelfer/tfel/issues/1281>

## Issue 1274: [mfront] abaqusexplicit, MFront orthotropy policy: the integration applies the inverse rotation, second axis read beyond the orientation in 2D

For more details, see <https://github.com/thelfer/tfel/issues/1274>

## Issue 1270: [mtest] @RotationMatrix<Euler> applies the inverse rotation, convention of @RotationMatrix undocumented, rotation matrix of the generic interface described by its transpose

For more details, see <https://github.com/thelfer/tfel/issues/1270>

## Issue 1264: [tfel] Installed headers that cannot be compiled, 2D microstructure-based homogenisation, Windows installation pages

For more details, see <https://github.com/thelfer/tfel/issues/1264>

## Issue 1262: [tfel-math] Second derivative of the determinant of a non-symmetric tensor, const scalar views of quantities, truncated exponent in the derivative of a**b

For more details, see <https://github.com/thelfer/tfel/issues/1262>

## Issue 1260: [mtest] Abaqus/Explicit stiffness in a rotated frame, incomplete orthotropic thermal expansion, tfel-check second precision, failed tangent operator comparison, overwritten JUnit reports

For more details, see <https://github.com/thelfer/tfel/issues/1260>

## Issue 1258: [mfront] generic MTest files of finite strain behaviours, cpptest and gnuplot interfaces, calculix MieheApelLambrechtLogarithmicStrainII strategy, dianafea failures and orthotropic behaviours, abaqus glue lock on Windows

For more details, see <https://github.com/thelfer/tfel/issues/1258>

## Issue 1256: [mfront] Model overriding parameters, bounds on main variables and on components, ScalarDamage in axisymmetrical generalised plane stress, keyword pages, Ksf of the harmonic sum flow, brick base classes exports, mfront-doc/mfront-query global options, MFrontLock

For more details, see <https://github.com/thelfer/tfel/issues/1256>

## Issue 1254: [tfel-math] Eigen solvers of symmetric tensors: wrong eigenvectors for equal or close eigenvalues, Cardano eigenvalues of syevc3, derivatives of isotropic functions of quantities, ConvertLogarithmicStrainTangentOperator

For more details, see <https://github.com/thelfer/tfel/issues/1254>

## Issue 1251: [cmake] Installed executables, libraries and Python modules without RUNPATH on Linux, madnex library not found, empty entries added by env.sh

For more details, see <https://github.com/thelfer/tfel/issues/1251>

## Issue 1242: [tests] Twenty-one C++ unit tests check no result

For more details, see <https://github.com/thelfer/tfel/issues/1242>

## Issue 1241: [ptest] Thirty of the 42 PTest pipe tests compare nothing

For more details, see <https://github.com/thelfer/tfel/issues/1241>

## Issue 1239: [cmake] Obsolete packaging files, TFEL_OPTIMISATION_FLAGS0, Debug flags, checks that never act, Cast3M 2026 header, minimum CMake version, exports of libTFELMFront

For more details, see <https://github.com/thelfer/tfel/issues/1239>

## Issue 1237: [cmake] Compiler printed by mfront --version, installed headers of the isotropic DSLs and of TFELCheck, dependencies of the installed CMake packages, static TFELMFrontDatabase package, CPack components

For more details, see <https://github.com/thelfer/tfel/issues/1237>

## Issue 1235: [tests] Copy-paste slips and vacuous checks in unit tests

For more details, see <https://github.com/thelfer/tfel/issues/1235>

## Issue 1233: [mfront] MFront DSL unit tests accept any exception

For more details, see <https://github.com/thelfer/tfel/issues/1233>

## Issue 1231: [tests] Commented-out cases in the Cazacu, LocalDataStructure, CxxTokenizerOffset and eigenvectors unit tests

For more details, see <https://github.com/thelfer/tfel/issues/1231>

## Issue 1229: [mfront] Expected-failure tests that fail for an unrelated reason

For more details, see <https://github.com/thelfer/tfel/issues/1229>

## Issue 1227: [mfront] Comparisons of the finite strain single crystal tests are commented out

For more details, see <https://github.com/thelfer/tfel/issues/1227>

## Issue 1225: [mtest] The fanderson and uanderson tests do not select their acceleration algorithm

For more details, see <https://github.com/thelfer/tfel/issues/1225>

## Issue 1223: [mtest] Thirty test files give the same variable twice in a @Test map

For more details, see <https://github.com/thelfer/tfel/issues/1223>

## Issue 1221: [mfront] Logarithmic strain shear tests: the shear-FZY-3D tests impose FZX, and the transposed cases compare too few stresses

For more details, see <https://github.com/thelfer/tfel/issues/1221>

## Issue 1219: [mfront] Strain tolerance of the axisymmetric shear tests of orthotropic elasticity

For more details, see <https://github.com/thelfer/tfel/issues/1219>

## Issue 1217: [tfel-glossary] Types, units and short descriptions of some glossary entries

For more details, see <https://github.com/thelfer/tfel/issues/1217>

## Issue 1215: [mfront] Installed GenericData and LSDYNA explicit headers, undefined declarations of the code generator and specific targets, madnex search paths documentation, pedantic bounds of parameters, castem21 keyword restrictions

For more details, see <https://github.com/thelfer/tfel/issues/1215>

## Issue 1213: [mfront] @RequireThermalExpansionCoefficientTensor is unusable: missing alias, documented syntax, castem finite strain, stype of the other interfaces

For more details, see <https://github.com/thelfer/tfel/issues/1213>

## Issue 1211: [tfel-system] Undefined SignalManager and NUMODIS declarations, MTestParser and ProcessManagerCommand constructors, mtest reference file generator, terminate handler of the tools, BurnUp glossary name in the docs

For more details, see <https://github.com/thelfer/tfel/issues/1211>

## Issue 1209: [tfel-math] Variable positions of Evaluator in diff() and tfel-check, fses::sort, vector::copy, GenType output, GTN flow stress, Mohr-Coulomb second derivative, eigen solvers on GPUs

For more details, see <https://github.com/thelfer/tfel/issues/1209>

## Issue 1207: [mfront] Logarithm of the stress-free expansion stretch (abaqus, ansys, calculix, lsdyna), castem tangent operators in 1D and with FiniteRotationSmallStrain, cyrano hypothesis identifier, repeated @Test keys

For more details, see <https://github.com/thelfer/tfel/issues/1207>

## Issue 1205: [mfront] comsol error codes and initialisation, aster orthotropic cohesive zone models, bounds symbols of array elements, cyrano plane stress header, zmat arrays of tensors

For more details, see <https://github.com/thelfer/tfel/issues/1205>

## Issue 1203: [tfel-math] Isotropic moduli assignment, stensor products on GPUs, Gauss-Kronrod half-infinite error estimate, QR with more rows than columns, Levenberg-Marquardt factor, physical constants digits

For more details, see <https://github.com/thelfer/tfel/issues/1203>

## Issue 1201: [tfel-utilities] CxxTokenizer numbers and integers, string algorithms, TextData comments and infinities, ProcessManager and SignalManager children, tfel-doc --split

For more details, see <https://github.com/thelfer/tfel/issues/1201>

## Issue 1199: [mfront] Code block option ".+", @PhysicalBounds on array elements, thermal expansion options, external files registered twice, isotropic DSLs last iteration, bricks and generic behaviours with quantities

For more details, see <https://github.com/thelfer/tfel/issues/1199>

## Issue 1197: [mtest] Acceleration algorithm parameters, Anderson method orders above 22, castem_umat optional material properties, mtest Python options

For more details, see <https://github.com/thelfer/tfel/issues/1197>

## Issue 1195: [mfront] madnex search paths: file extension, behaviours and models, knowledge type; madnex export of shared dependencies

For more details, see <https://github.com/thelfer/tfel/issues/1195>

## Issue 1193: [mtest] End of input in the MTest and PTest parsers, ansys input file example of orthotropic behaviours, misleading interface messages

For more details, see <https://github.com/thelfer/tfel/issues/1193>

## Issue 1191: [mfront-query] Names of the DSL options in the help, strain measure output, specific targets rules and help texts

For more details, see <https://github.com/thelfer/tfel/issues/1191>

## Issue 1189: [mfront] Cazacu Pipe convention, StrainRateSensitive derivative, @Initialize, command line keywords, StringArray defaults, array bounds, log stream, fortran03/python/octave/java interfaces and parser messages

For more details, see <https://github.com/thelfer/tfel/issues/1189>

## Issue 1187: [mfront] castem logarithmic strain helpers, stensor absolute value, DSLBase helpers, FSES return codes, TestDescription constructor, exported constants precision, tfel-check columns and ABAQUSEXPLICIT_NTHREADS

For more details, see <https://github.com/thelfer/tfel/issues/1187>

## Issue 1185: [mtest] dianafea wrapper shear order, tfel-check JUnit escaping, raw stream readers, Python docstrings, documentation links and tutorial examples

For more details, see <https://github.com/thelfer/tfel/issues/1185>

## Issue 1183: [cmake] Static library names, test dependencies, C++ standard, static MFront package, environment scripts, Doxygen install, installed MFront headers and exports

For more details, see <https://github.com/thelfer/tfel/issues/1183>

## Issue 1181: [python] mfront.database entry points, mtest initial values and depths, tfel.material imports, mfm-test-generator, mfm --filter-by-type and MTest files on failure

For more details, see <https://github.com/thelfer/tfel/issues/1181>

## Issue 1179: [mfront] Wrong or misleading error messages and documentation of DSLs, tools, material property interfaces, stress criteria, MTest bindings and tfel-check

For more details, see <https://github.com/thelfer/tfel/issues/1179>

## Issue 1177: [mfront] ansys, comsol, dianafea, castem, aster and cyrano interfaces: material properties layout, tangent operator conventions, 2D sizes, plane stress checks, Gibiane examples and messages

For more details, see <https://github.com/thelfer/tfel/issues/1177>

## Issue 1175: [installation] Doxygen deployment, keyword descriptions, installation pages, maintainer scripts, generic interface and math docs, quantity tests option, physical constants and glossary generators

For more details, see <https://github.com/thelfer/tfel/issues/1175>

## Issue 1173: [mfront] abaqus and amitex input examples, cyrano header, ansys/lsdyna/dianafea example files and zmat interface

For more details, see <https://github.com/thelfer/tfel/issues/1173>

## Issue 1171: [mfront-query] File extensions of mfront.write and mtest.write, mfront-query lookups, bounds and specific targets, mfront-doc descriptions and constants, InelasticFlowBase include

For more details, see <https://github.com/thelfer/tfel/issues/1171>

## Issue 1169: [tfel-tests] tfel-doc output, log and colors, TFEL/Tests XML output and exports, copy-paste slips, JUnit report names, TextData::skipLines, FSAlgorithm loop and strided iterator assignment

For more details, see <https://github.com/thelfer/tfel/issues/1169>

## Issue 1167: [tfel-utilities] CxxTokenizer, Data arrays, reserved keywords, systemCall::copy, wfstream, TFEL/System messages and tfel-check end of file and empty_output

For more details, see <https://github.com/thelfer/tfel/issues/1167>

## Issue 1165: [tfel-math] Headers and parser: parenthesised conditions, stensor setComponent/getComponent, isqrt3 with MSVC, square_root of quantities, GPU attribute, ATLAS binding, HasRandomAccessIterator, self-contained material headers

For more details, see <https://github.com/thelfer/tfel/issues/1165>

## Issue 1163: [python] Bindings: CurrentStateVector, tfel.math operators, versioned packages, CMakeGenerator, return value policies, Hill tensors, DSLFactory and mergeTargetsDescription

For more details, see <https://github.com/thelfer/tfel/issues/1163>

## Issue 1161: [mtest] MTest and PTest: ptest keywords help, array component names, orthotropic generic stress, mistral defaults, PTest output and jacobian, algorithm names, installed and exported classes

For more details, see <https://github.com/thelfer/tfel/issues/1161>

## Issue 1159: [tfel-material] Finite strain tangent operators, logarithmic strain handler, Hosford/Barlat with quantities, bounds checks, Gauss-Kronrod, broyden, parser and LU messages

For more details, see <https://github.com/thelfer/tfel/issues/1159>

## Issue 1157: [mfront] castem, aster, abaqus, ansys and abaqusexplicit interfaces: LogarithmicStrain1D, sub-stepping, 2D finite strain fallbacks, tangent operator layouts and numerical comparison

For more details, see <https://github.com/thelfer/tfel/issues/1157>

## Issue 1155: [mfront] DSLs and generated code: modelling hypotheses intersection, braceless assignments, output operators, brick symmetries, code blocks, error messages and debug comments

For more details, see <https://github.com/thelfer/tfel/issues/1155>

## Issue 1153: [tfel-math] Quantities and views: qt_ref assignment, change_basis, positive parts, runtime matrix views, arrays of views, tmatrix products, FSES NaN, Hashin-Shtrikman shear bounds, Anderson weights

For more details, see <https://github.com/thelfer/tfel/issues/1153>

## Issue 1151: [mfront] Generated code: integration variable derivatives, tangent operator blocks, generic gradients, relocation model, brick arrays, MultipleIsotropicMisesFlows, behaviour variables and ℝ₊ bounds

For more details, see <https://github.com/thelfer/tfel/issues/1151>

## Issue 1149: [mtest] Initial elastic stiffness of Abaqus/Explicit behaviours: halved shear terms and 3D shear columns

For more details, see <https://github.com/thelfer/tfel/issues/1149>

## Issue 1147: [tfel-math] Formula parsers, kriging and LUSolve: copy and arguments of diff, Cast3M functions without argument, integer division by zero, rebuilt and empty kriging, work vector size

For more details, see <https://github.com/thelfer/tfel/issues/1147>

## Issue 1145: [tfel-math] Headers: abs of t2tost2, st2tost2 conversion in 1D, eigen tensors of quantities, tmatrix, array derivatives, copy, strided maps and indexing policies

For more details, see <https://github.com/thelfer/tfel/issues/1145>

## Issue 1143: [mfront] DSLs: bricks, behaviour variables, @TangentOperator, initialisation values, targets.lst, overriding increments and bounds

For more details, see <https://github.com/thelfer/tfel/issues/1143>

## Issue 1141: [tfel-utilities] tfel-check shall_fail and directory configurations, rstream::getLine, CxxTokenizer, Data reading and comparison, ArgumentParser

For more details, see <https://github.com/thelfer/tfel/issues/1141>

## Issue 1139: [python] Bindings: evtypes, ST2toST2 subscripts, homogenisation bounds, setRotationMatrix, setStrainMeasure, umatxx, Anderson algorithms and Data

For more details, see <https://github.com/thelfer/tfel/issues/1139>

## Issue 1137: [mfront] Generated code: amitex template, arrays of state variables and gradients, profiling, @Relocation, rotation functions and MTest files on failure

For more details, see <https://github.com/thelfer/tfel/issues/1137>

## Issue 1135: [tfel-material] NUMODIS and slip systems: Schmid factors, installed headers, exports, HCP families and symmetries

For more details, see <https://github.com/thelfer/tfel/issues/1135>

## Issue 1133: [tfel-math] Powell dog leg solvers, scalar Newton-Raphson, TinyMatrixSolve/Invert, CubicRoots, runtime_array output and Fischer-Burmeister derivatives

For more details, see <https://github.com/thelfer/tfel/issues/1133>

## Issue 1131: [mfront] Hooke stress potentials: plane stress jacobian, generic_prediction_operator option and broken state tests

For more details, see <https://github.com/thelfer/tfel/issues/1131>

## Issue 1129: [mtest] Wrappers of the generic, aster, castem, ansys, abaqusexplicit and castem_umat behaviours

For more details, see <https://github.com/thelfer/tfel/issues/1129>

## Issue 1127: [tfel-system] rmdir following symbolic links, mkdir, binary_read, ThreadPool, ThreadedTaskResult, pipes and ELF symbols

For more details, see <https://github.com/thelfer/tfel/issues/1127>

## Issue 1125: [mfront] Tangent operators of cohesive zone models (castem, aster, MTest) and calculix interface

For more details, see <https://github.com/thelfer/tfel/issues/1125>

## Issue 1123: [mfront] StandardElastoViscoPlasticity brick: flow identifiers, flow criteria coefficients, theta factors and hardening rules

For more details, see <https://github.com/thelfer/tfel/issues/1123>

## Issue 1121: [tfel-material] Hill tensors, orientation averages, Plate convention and altered stiffness tensors in axisymmetrical generalised plane stress

For more details, see <https://github.com/thelfer/tfel/issues/1121>

## Issue 1119: [mfront] ansys and abaqusexplicit interfaces: finite strain strategies, plane stress, packaging and parallelisation

For more details, see <https://github.com/thelfer/tfel/issues/1119>

## Issue 1117: [mfront] Interfaces of material properties and models: exports, one-line functions, bounds, temperature of castem models and comsol

For more details, see <https://github.com/thelfer/tfel/issues/1117>

## Issue 1115: [tfel-math] Eigenvalue sorting, Harari solver, positive/negative parts, convert, tpld and fromRotationMatrix on GPU

For more details, see <https://github.com/thelfer/tfel/issues/1115>

## Issue 1113: [mfront] StandardElastoViscoPlasticity brick: nucleation, porosity name, porosity jacobian and porous stress criteria

For more details, see <https://github.com/thelfer/tfel/issues/1113>

## Issue 1111: [mtest] Crash without behaviour, time step at the end of a period, events, NaN results and cubic pipe elements

For more details, see <https://github.com/thelfer/tfel/issues/1111>

## Issue 1109: [mfront] Runge-Kutta DSLs and solvers: rk4 last stage, external models, error estimates and final time

For more details, see <https://github.com/thelfer/tfel/issues/1109>

## Issue 1107: [mfront] Normalisation factors, physical bounds and MultipleIsotropicMisesFlows in the implicit and isotropic DSLs

For more details, see <https://github.com/thelfer/tfel/issues/1107>

## Issue 1105: [docs] Examples rejected by mfront and mtest, wrong keyword names and broken links

For more details, see <https://github.com/thelfer/tfel/issues/1105>

## Issue 1103: [tfel-math] Crash, operator precedence, derivative and integer literal errors of the formula parsers

For more details, see <https://github.com/thelfer/tfel/issues/1103>

## Issue 1101: [tfel-material] Wrong second derivatives, NaN and single precision build errors of several stress criteria

For more details, see <https://github.com/thelfer/tfel/issues/1101>

## Issue 1099: [mfront] Wrong shear components of stress-free expansions in several behaviour interfaces

For more details, see <https://github.com/thelfer/tfel/issues/1099>

## Issue 1097: [tfel-math] `x /= 2` sets every component of arrays and views to zero

For more details, see <https://github.com/thelfer/tfel/issues/1097>

## Issue 1095: [mfront] `rk54` and `rk4` compute the final stress with the stiffness tensor of the middle of the time step

For more details, see <https://github.com/thelfer/tfel/issues/1095>

## Issue 1093: [5.2] Regressions since TFEL 5.1

For more details, see <https://github.com/thelfer/tfel/issues/1093>

## Issue 1089: Add debug mode + hardening in the test matrix

The sanitizers options have been modularised to allow the continuous
integration to enable sanitizers individually. For more details, see
<https://github.com/thelfer/tfel/issues/1089>

## Issue 1038: [tfel-check] don't execute cleaning tasks when a test fails

For more details, see <https://github.com/thelfer/tfel/issues/1038>

## Issue 1037: [tfel-check] add a `--clean` option to select whether or not cleaning is done

For more details, see <https://github.com/thelfer/tfel/issues/1037>

## Issue 1036: [tfel-check] Add timeout option

For more details, see <https://github.com/thelfer/tfel/issues/1036>

## Issue 1023: [tfel-config] add an option `--cmake-prefix-path`

For more details, see <https://github.com/thelfer/tfel/issues/1023>

## Issue 1016: [mfront] Variables have the same name in the `dgibi` file generated by `MFront`, for using the FEM program `Cast3M`

For more details, see <https://github.com/thelfer/tfel/issues/1016>

## Issue 1014: [cmake] Add a `build-tests` target
 
For more details, see <https://github.com/thelfer/tfel/issues/1014>

## Issue 1013: [tfel-math] Refine `QuantityConcept` to take mutability into account

For more details, see <https://github.com/thelfer/tfel/issues/1013>

## Issue 1012: [github-actions] Add testing of `TFEL` with `madnex`

For more details, see <https://github.com/thelfer/tfel/issues/1012>

## Issue 1010: [clang-tidy] fix warnings related to the google-explicit-constructor check

For more details, see <https://github.com/thelfer/tfel/issues/1010>

## Issue 1007: [github-actions] Add continuous integration for the generic-parallel interface with CUDA backend

For more details, see <https://github.com/thelfer/tfel/issues/1007>

## Issue 1006: [github-actions] Add tests based on `tfel-check`

For more details, see <https://github.com/thelfer/tfel/issues/1006>

## Issue 1004:  [github-actions] Add support actions based on Visual Studio

For more details, see <https://github.com/thelfer/tfel/issues/1004>

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

For more details, see <https://github.com/thelfer/tfel/issues/922>

## Issue 890: [mfront][homogenization] Add Ponte Castaneda 1992 non linear scheme

For more details, see <https://github.com/thelfer/tfel/issues/890>

## Issue 905: [TFEL/Math] Pointer-heavy CoalescedView

For more details, see <https://github.com/thelfer/tfel/issues/905>

## Issue 853: [tfel-fcheck] Have the possibility to launch multiple call to `tfel-check` simultaenously in a CI context

For more details, see <https://github.com/thelfer/tfel/issues/853>
