---
title: The `generic-parallel` interface for material properties
author: Thomas Helfer
date: 28/04/2026
lang: en-EN
link-citations: true
colorlinks: true
geometry:
  - margin=2cm
papersize: a4
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

This page describes the `generic-parallel` interface.

The `generic-parallel` interface has been designed to be compatible with
current parallel programming models (see the `Backends` section).

# Prototypes of the generated functions

The `generic-parallel` interface for material properties generates two
functions. Those functions can be used:

- directly by including the generated headers and linking with the
  generated library.
- indirectly by loading the generated function using system calls like
  `dlopen`/`dlsym` on `Unix` systems or `LoadLibrary`/`GetProcAddress`
  on `Windows`.

In addition, the `generic-parallel` interface also exports:

- symbols describing the material property (names of the inputs, list of
  parameters, default values, etc.),
- a function allowing to modify at runtime the value of the parameters
  of the material property, if modification of parameters is allowed
  (see the DSL options `parameters_as_static_variables` options).

## First prototype supporting data with strides

The first generated function matches the following prototype:

~~~~{.cxx}
void (*)(mfront_gmp_OutputStatus* const,       // output status
         mfront_gmp_real* const,               // output values
         const mfront_gmp_size_type,           // output stride
         ...,                                  // extra arguments specific to the backend considered
         const mfront_gmp_real* const,         // values of the arguments
         const mfront_gmp_size_types* const,   // strides of the arguments
         const mfront_gmp_size_type,           // number of arguments
         const mfront_gmp_size_type,           // number of points
         const mfront_gmp_OutOfBoundsPolicy);  // out of bounds policy
~~~~

The `mfront_gmp_OutputStatus` structure and the
`mfront_gmp_OutOfBoundsPolicy` enumeration type are described in the
page dedicated to the [`generic` interface for material
properties](generic-parallel-material-property-interface.html). The
`mfront_gmp_real` type is by default an alias to double precision
floating point numbers.

If the number of points is null, no computation is made.

A null stride means that the associated argument is uniform. The stride
of the output can be null only if all the strides of all the arguments
are null, otherwise an error is reported.

Most backends will treat the case where all strides are equal to one as
a special case and will use an optimized implementation.

## Second prototype

The second generated function matches the following prototype:

~~~~{.cxx}
void (*)(mfront_gmp_OutputStatus* const,       // output status
         mfront_gmp_real* const,               // output values
         ...,                                  // extra arguments specific to the backend considered
         const mfront_gmp_real* const,         // values of the arguments
         const mfront_gmp_size_type,           // number of arguments
         const mfront_gmp_size_type,           // number of points
         const mfront_gmp_OutOfBoundsPolicy);  // out of bounds policy
~~~~

This function assumes that the values of the arguments and the values of
the output are stored contiguously in memory. In other words, this
prototype is equivalent to the the first prototype when all strides are
equal to one.

# Backends

Backends are associated with parallel programming models.

## `CUDA` backend

### Example of usage

Let `UO2_ShearModulus.mfront` be an implementation of a material
property computing the shear modulus of uranium dioxide, which depends
on the temperature and on the porosity. This file can be compiled as
follows:

~~~~{.bash}
$ mfront --obuild --configuration-file=config-cuda-XXXX.json \
         --interface=generic-parallel UO2_ShearModulus.mfront
The following library has been built :
- libGenericParallelUO2-cuda.so :  UO2_ShearModulus UO2_ShearModulus2
~~~~

where the configuration file `config-cuda-XXXX.json` provide the
required information to call a `CUDA` compiler (`nvcc` or `clang++`)
with the appropriate flags. This file may also contain the options
associated with the `CUDA` backend, see below. Depending on the parallel
configurations selected during the `cmake` configuration steps, two
configuration files may be installed: `config-cuda-nvcc.json` and
`config-cuda-nvhpc.json`.

`UO2_ShearModulus` implements the first prototype, while
`UO2_ShearModulus2` implements the second.

The following code shows how to call the function `UO2_ShearModulus`. In
this example, the porosity is assumed to be uniform and has thus a
stride equal to zero. For the sake of simplicity, memory allocation on
the host and on the device are handled by the [Thrust
library](https://developer.nvidia.com/thrust).

~~~~{.cxx}
thrust::host_vector<double> G;
thrust::host_vector<double> T = {300, 500, 300, 800};
thrust::host_vector<double> f = {0.1};

thrust::device_vector<double> d_T(T);
thrust::device_vector<double> d_f(f);
thrust::device_vector<double> d_G(T.size());

auto output = mfront_gmp_OutputStatus{};
const auto policy = GENERIC_MATERIALPROPERTY_NONE_POLICY;
const auto args = std::array<double *, 2u>{thrust::raw_pointer_cast(d_T.data()),
                                     thrust::raw_pointer_cast(d_f.data())};
const auto args_stride = std::array<mfront_gmp_size_type, 2u>{1, 0};
UO2_ShearModulus(&output, thrust::raw_pointer_cast(d_G.data()), 1,
                 args.data(), args_stride.data(), 2, 4, policy);
G = d_G;
~~~~

The following remarks can be made:

- the user is responsible for consistently passing pointers to memory
  allocated on the device to the `UO2_ShearModulus` function, including
  uniform values. Shared memory can also be used.
- The [Thrust library](https://developer.nvidia.com/thrust) is not used
  by the `CUDA` backend. It is only used in the example for simplicity.
- The functions generated by the `CUDA` backend and the presented
  example can be compiled with both `nvcc` and `clang++`.

#### Example of configuration file for `nvcc`

The following configuration file exemplifies how to use the `nvcc`
compiler to build the source code generated by the `CUDA` backend

~~~~{.json}
compilation_options : {
  cuda : {
    compiler: "/usr/local/cuda-12.8/bin/nvcc",
    compilation_flags: {"-O2 -std=c++20 -diag-suppress 20012",
       "--expt-relaxed-constexpr", "-Xcompiler -fPIC"}
  }
}
~~~~

#### Example of configuration file for `clang++`

The following configuration file exemplifies how to use the `clang++`
compiler to build the source code generated by the `CUDA` backend:

~~~~{.json}
compilation_options : {
  cuda : {
    compiler: "clang++",
    compilation_flags: {"-O3 -std=c++20 -march=native",
      "-x cuda  --cuda-compile-host-device --cuda-gpu-arch=sm_86",
      "--cuda-path=/usr/local/cuda-12.8/ -fPIC -DPIC"}
  }
},
linking_options: {
  linker_flags: "-L/usr/local/cuda-12.8/lib64 -lcudart -ldl -lrt -pthread"
}
~~~~

### Options

The only option available is `number_of_threads_per_block`, the number
of threads per block. By default, the value used is \(64\). This number
is generally chosen a multiple of the number of threads per warps, which
is typically \(32\) or \(64\). Typical values are thus \(64\), \(128\)
and \(256\).

### Internal details

When required, the strides are stored in constant memory on the device.

If the material property exposes parameters, those are stored in a
global variable on the host (the CPU). For efficiency, parameters are
copied in constant memory on the device when evaluating the material
property. This mechanism allows to keep the flexibility provided by
parameters: this flexibility is required to perform sensitivity
analyses, uncertainty propagation studies or perform a new
identification. Note that parameters' handling can still be disabled by
setting the `parameters_as_static_variables` DSL option to `true`.

The errors are handled by allocating a managed array of integers
associated with all possible kind of errors. This array is only accessed
if an error occurs, minimizing the cost of error handling. On output,
the user knows if an error occurred, but not where this errors occurred.
For instance, the `UO2_ShearModulus` function may report that one
temperature passed on input was negative at one evaluation point. For
material properties, errors are currently associated with violation of
bounds and physical bounds. Those tests are disabled by setting the
`disable_runtime_checks` DSL optionto `true`. Note that this option also
disables other checks performed on the host (notably regarding the
number of arguments passed).

## `HIP` backend

The `HIP` backend is similar to the `CUDA` backend.

## Parallel STL backend (`stdpar`)

### Example of usage

Let `UO2_ShearModulus.mfront` be an implementation of a material
property computing the shear modulus of uranium dioxide, which depends
on the temperature and on the porosity. This file can be compiled as
follows:

~~~~{.bash}
$ mfront --obuild --configuration-file=config-stdpar.json \
         --interface=generic-parallel UO2_ShearModulus.mfront
The following library has been built :
- libGenericParallelUO2-stdpar.so :  UO2_ShearModulus UO2_ShearModulus2
~~~~

where the configuration file `config-stdpar.json` provide the required
information to compile the generated source files and also selects the
execution policy to be used (see below).

`UO2_ShearModulus` implements the first prototype, while
`UO2_ShearModulus2` implements the second.

~~~~{.cxx}
auto G = std::vector<double>(4);
const auto T = std::vector<double>{300, 500, 300, 800};
const auto f = std::vector<double>{0.1};
auto output = mfront_gmp_OutputStatus{};
const auto policy = GENERIC_MATERIALPROPERTY_NONE_POLICY;
const auto args = std::array<const double *, 2u>{T.data(), f.data()};
const auto args_strides = std::array<mfront_gmp_size_type, 2u>{1, 0};
UO2_ShearModulus(&output, G.data(), 1, args.data(), args_strides.data(), 2, 4,
                 policy);
~~~~

#### Example of configuration file for `g++`

The following configuration file exemplifies how to use the `clang++`
compiler to build the source code generated by the `stlpar` backend:

~~~~{.json}
interfaces_options: {
  generic-parallel: {
    backend: {stlpar: {execution_policy: "parallel_unsequenced_policy"}}
  }
},
compilation_options : {
  cxx : {
    compiler: "g++",
    compilation_flags: "-O2 -std=c++20 -march=native" 
  }
},
linking_options : {
  linker_flags : "-ltbb"
}
~~~~

#### Example of configuration file for `nvhcp`

The following configuration file exemplifies how to use the `nvhcp`
compiler to build the source code generated by the `stlpar` backend and
execute the computation of the material property on the device:

~~~~{.json}
interfaces_options: {
  generic-parallel: {
    backend: {stlpar: {execution_policy: "parallel_unsequenced_policy"}}
  }
},
compilation_options : {
  cxx : {
    compiler: "nvc++",
    compilation_flags: "-O2 -stdpar=gpu -std=c++20 -march=native -gpu=sm_89" 
  }
},
linking_options : {
  linker_flags: "-stdpar=gpu"
}
~~~~

### Options

The only option available is `execution_policy`, which can have one of
the following values:

- `sequenced_policy` (or `seq`)
- `unsequenced_policy` (or `unseq`)
- `parallel_policy` (or `par`)
- `parallel_unsequenced_policy` (or `par_unseq`)

The exact meaning of those policies are implementations defined and may
depend on compiler flags used. See [this
page](https://en.cppreference.com/cpp/algorithm/execution_policy_tag_t)
for details.

### Notes

- `sequenced_policy` denotes a standard sequential loop.
- `unsequenced_policy` seems generally associated to vectorization.
- `parallel_policy` and `parallel_unsequenced_policy` generally rely on
  a multithreading approach. This note applies to the current
  implementations provided by the `Visual Studio`, `gcc`, `clang++` or
  `icpx` compilers and the standard libraries they rely on.
- The compiler flag `-stdpar=gpu` provided by the `nvhpc` compiler has
  two effects:
  - all memory allocations made by standard containers (including
    `std::vector`) are managed: eventual memory transfers between the
    host (the CPU) and the device (the GPU) are done transparently.
  - the parallel algorithms are executed on the device (the GPU).
- `libstdc++` implements the parallel STL algorithms on the top of the
  Threading Building Blocks library and requires an explicit link to
  this library.

## `SYCL` backend

[`SYCL`](https://khronos.org/sycl) is an open cross-platform abstraction
layer that enables code for heterogeneous and offload processors. `SYCL`
allows mixing code on CPUs (leveraging multithreading), GPUs and
accelerators.

The `SYCL` backend has been tested with the following compilers:
`INTEL`' `icpx` and `AdaptativeCpp`'s `accp`.

### Example of usage

~~~~{.cxx}
//! \brief type of the memory allocator
using usm_allocator_type =
    sycl::usm_allocator<double, sycl::usm::alloc::host>;
//! \brief a simple alias for a vectors
using usm_vector = std::vector<double, usm_allocator_type>;
//
auto output = mfront_gmp_OutputStatus{};
//
auto Q = sycl::queue{sycl::gpu_selector_v};
auto usm_allocator = usm_allocator_type{Q};
//
auto E = usm_vector(4 * 3, Q);
const auto T = usm_vector({300, -300, 500, -500, 300, -300, 800, -800}, Q);
//
auto d_E = to_device(E, Q); // function transferring data to the device
auto d_T = to_device(T, Q);
//
const auto policy = mfront_gmp_OutOfBoundsPolicy{};
const auto args = std::array<const double *, 1u>{d_T.get()};
const auto args_strides = std::array<mfront_gmp_size_type, 1u>{2};
Inconel600_YoungModulus(&output, d_E.get(), 3, &Q, args.data(),
                        args_strides.data(), 1, 4, policy);
~~~~

### Specific argument

The `SYCL` backend adds one specific argument to the prototype of the
functions generated by `MFront`: a pointer to the execution queue.

### Backend options

This backend only has one option named `data_transfer_policy`.

#### Data transfer policy

Two policies regarding data transfer between host and device are
available:

- `automatic`: this policy may automatically trigger a copy of the data
  if needed. No copy is made if the data are already on the device or in
  shared memory.
- `none`: this policy assumes that the data are already on the device on
  in shared memory.

> ** Providing data on the device**
> 
> Data copy in `SYCL` are asynchronous operations. Thus, if the
> data are provided on the device, the user is responsible for ensuring
> that the data are effectively copied before calling `MFront`'s generated
> functions.

# Tests

Unlike other material property interfaces, the tests for the
`generic-parallel` interface is performed in the installation directory.
This allows to tests various configurations that may use various `C++`
compilers, a feature unsupported by the `cmake` build system.

The list of configurations tested must be specified by defining a list
named `generic-parallel-mp-tests-configurations` when invoking `cmake`
as follows:

~~~~
cmake -Dgeneric-parallel-mp-tests-configurations="cuda-nvcc;cuda-clang"
~~~~

The following tests configurations are currently supported:

- `cuda-nvcc`: compilation of `CUDA` files using the `nvcc` compiler.
- `cuda-clang`: compilation of `CUDA` files using the `clang++`
  compiler.
- `hip-hipcc`: compilation of `HIP` files using the `hipcc` compiler.
- `hip-clang`: compilation of `HIP` files using the `clang++` compiler.
- `hip-hipcc`: compilation of `HIP` files using the `hipcc` compiler.
- `stlpar-parunseq-gcc`:
- `stlpar-parunseq-clang`:
- `stlpar-parunseq-nvhpc-gpu`:
- `stlpar-parunseq-icpx`:
- `sycl-default-icpx`: compilation of `SYCL` files using the `icpx` compiler.
- `sycl-default-acpc`: compilation of `SYCL` files using the `acpc`
  compiler on the CPU.
- `sycl-gpu-acpc`: compilation of `SYCL` files using the `acpc`
  compiler on the GPU using the `generic` target.

### Notes

For configurations based on HIP, the variable `HIP_PLATFORM` must be
passed to `cmake` or defined the environment. The value of this variable
must be either `nvidia` or `amd`. If `HIP_PLATFORM` is `nvidia`, the
`CUDA` platform must be available.
