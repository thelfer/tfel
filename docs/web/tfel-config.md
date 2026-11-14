---
title: Overview of the TFEL/Config library
author: Thomas Helfer
date: 12/06/2026
lang: en-EN
numbersections: true
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
lstPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
header-includes:
bibliography: bibliography.bib
csl: iso690-numeric-en.csl
---

The `TFEL/Config` library is mostly used internally by `TFEL` and
provides some information related to the project.

# `getTFELDefaultSubstitutions`: TFEL's default substitutions

The `getTFELDefaultSubstitutions` function returns some default
substitutions used by `MFront` when reading configuration files and
`tfel-check` when analysing `.check` files. The keys listed below are
delimited by the `@` character in those files and in the map returned
by this function, e.g. `@TFEL_CXX_STANDARD@`.

Only `TFEL_CXX_STANDARD` is always defined. The other keys are only
defined if `TFEL` is configured with a configuration of the
`generic-parallel` interface which uses them, i.e. listed in the
`generic-parallel-configurations` variable of `cmake`:

- `TFEL_GXX_COMPILER`: `stlpar-parunseq-gcc`,
- `TFEL_CLANGXX_COMPILER`: `cuda-clang`, `hip-clang` or
  `stlpar-parunseq-clang`,
- `TFEL_ICPX_COMPILER`: `stlpar-parunseq-icpx` or `sycl-default-icpx`,
- `TFEL_ACPP_COMPILER`: `sycl-default-acpp` or `sycl-gpu-acpp`,
- `TFEL_NVHPC_COMPILER`: `stlpar-parunseq-nvhpc-gpu`,
- `TFEL_HIPCC_COMPILER`, `TFEL_HIPCC_FLAGS` and `TFEL_HIP_INCLUDE_DIR`:
  `hip-hipcc` or `hip-clang`,
- `TFEL_NVCC_COMPILER`, `TFEL_CUDA_PATH` and `TFEL_CUDA_LIBRARY_DIR`:
  `cuda-nvcc`, `cuda-clang`, or `hip-hipcc` and `hip-clang` on the
  NVIDIA platform (`HIP_PLATFORM` set to `nvidia`).

Without such a configuration, these keys are not substituted, and the
variables `TFEL_xxx_COMPILER` passed to `cmake` are not used.

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
