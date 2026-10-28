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

This page describes the `generic-parallel` interface for material
properties.

The `generic-parallel` interface has been designed to be compatible with
current parallel programming models (see the `Backends` section).

# Generated functions

For a material property `S` (the name of the function of the `generic`
interface, `<material>_<law>` or `<law>`), the `generic-parallel`
interface generates two functions, declared by the generated header:

~~~~{.cxx}
// evaluation of the material property on a batch of points
int S_generic_parallel(mfront_gpmp_OutputStatus* const,
                       const mfront_gpmp_BatchData* const);
// analysis of the statistics of an evaluation (see below)
int S_generic_parallel_report(mfront_gpmp_OutputStatus* const,
                              const mfront_gpmp_Statistics* const);
~~~~

Both functions return the status of the output status. The data
structures are declared in
`MFront/GenericParallelMaterialProperty/MaterialProperty.h`, with the
types of the pointers to those functions
(`mfront_gpmp_BatchMaterialPropertyPtr` and `mfront_gpmp_ReportPtr`).
The name of a material property can't contain `_generic_parallel`.

Those functions can be used:

- directly by including the generated headers and linking with the
  generated library.
- indirectly by loading the generated function using system calls like
  `dlopen`/`dlsym` on `Unix` systems or `LoadLibrary`/`GetProcAddress`
  on `Windows`.

In addition, the `generic-parallel` interface also exports:

- the symbols describing the material property exported by the `generic`
  interface (names of the inputs, list of parameters, default values,
  etc.), and the `S_mfront_interface` symbol, equal to `GenericParallel`.
  The library does not contain the function `S` of the `generic`
  interface;
- the `S_generic_parallel_api_version` symbol (an `unsigned short`),
  equal to the `MFRONT_GPMP_API_VERSION` macro (`1`). This symbol tells
  that the library contains the functions of batches of points. It only
  changes with an incompatible change of those functions: a calling code
  must refuse any version it does not know;
- the `S_generic_parallel_backend` symbol, which gives the backend of the
  library (`cuda`, `hip`, `stlpar` or `sycl`), for information;
- the `S_generic_parallel_execution_policy` symbol (a `const char*`),
  which gives the identifier of the execution policy expected by the
  generated function (see below);
- the `S_generic_parallel_memory_space` symbol (a `const char*`), which
  tells where the values and the statuses of the points must be located
  (`host`, `device` or `any`). The statistics must
  also be readable by the host (see below). The
  name of the backend does not suffice: for instance, the `stlpar` backend
  computes on the host or on a GPU depending on the compiler. This memory
  space is set when the library is compiled (see
  `MFront/GenericParallel/MemorySpace.hxx`);
- a function allowing to modify at runtime the value of the parameters
  of the material property, if modification of parameters is allowed
  (see the `parameters_as_static_variables` DSL option).

## Data of a batch

The data of a batch are given by the `mfront_gpmp_BatchData` structure:

~~~~{.cxx}
typedef struct mfront_gpmp_BatchData {
  mfront_gmp_size_type number_of_points;
  mfront_gpmp_Field output;                 // output values
  const mfront_gpmp_ConstField* arguments;  // array read on the host
  mfront_gmp_size_type number_of_arguments;
  mfront_gmp_OutOfBoundsPolicy out_of_bounds_policy;
  int* statuses;                            // optional, code of each point
  mfront_gpmp_Statistics* statistics;       // optional, see below
  const mfront_gp_AbstractBackendExecutionPolicy* execution_policy;  // see below
} mfront_gpmp_BatchData;
~~~~

The output and each argument are given by a pointer to their first value
(`values`) and by the stride between two points (`point_stride`): the
value at the `i`-th point is `values[i * point_stride]`. A null stride
means that the argument is uniform. The stride of the output can only be
null for a batch of one point. The array of the arguments is read on the
host, and may be null if the material property has no argument. The
`mfront_gmp_OutOfBoundsPolicy` enumeration type is described in the page
dedicated to the [`generic` interface for material
properties](generic-material-property-interface.html). The
`mfront_gmp_real` type is by default an alias to double precision
floating point numbers.

The evaluation at each point is the evaluation of the `generic`
interface: same checks, same status. When several checks fail at a point,
the first check making the evaluation fail is retained, otherwise the
last bound violated with the warning policy, as with the `generic`
interface. At a point where the evaluation fails (status `-1`), the
output is `NaN`. At a point where the result is not finite (status
`-4`), the output is the computed value. If the `statuses` array is
given, the status of each point is written in it: `0`, `1`, `-1` or
`-4`, with the meaning of the status of the `generic` interface. It is
located in the memory space of the library, as the values, and is not
written if an error of the batch is detected before the submission of
the computations.

Contrary to the `generic` interface, `errno` is not used to detect
errors: it is thread-local and does not exist on GPUs. A result which is
not finite (`nan` or infinite) is reported by the status `-4`. The value
of `errno` is restored before returning.

An empty batch is accepted without any check: nothing is read, written
or submitted, and the status is
`0`. The arrays and the execution policy may then be null.

All the backends treat a batch of one point, and the case where all the
strides are equal to one, with a kernel without strides.

## Execution policies and statistics

The `execution_policy` and `statistics` members of the data of the batch
have the same meaning on all the backends:

- `execution_policy`: execution policy of the backend of the library
  (`mfront::generic_parallel::CUDABackendExecutionPolicy`, etc., declared
  in the headers of `MFront/GenericParallel`), or null for the default
  policy of the backend: the default stream for `CUDA` and `HIP`. The
  `SYCL` backend has no default policy: a null policy is refused (status
  `-5`), as a null queue. A policy of another backend is refused
  (status `-5`): the identifier of the expected policy is given by the
  `S_generic_parallel_execution_policy` symbol. The generated function
  copies what it needs from the policy, which does not have to outlive
  the call. The C++ parallel algorithms only use the common options of the
  policy. The evaluation is ordered after the
  computations previously submitted to the queue of the policy, which may
  compute the arguments. With the `SYCL` backend, an out-of-order queue
  gives no order between the kernels: an asynchronous evaluation then
  requires an in-order queue (`sycl::property::queue::in_order`), and is
  refused with an out-of-order queue (status `-5`). A synchronous
  evaluation synchronizes an out-of-order queue before the kernel.
- `statistics`: statistics of the evaluation, or null (see below).

The common options of the policies (`BackendExecutionCommonPolicy`) are:

- `preserve_stack_size`: if true, the stack size of the threads of the
  device is never modified and a runtime error (status `-7`) is reported
  if it is too small. Otherwise, the `CUDA` and `HIP` backends increase
  the stack size if the kernel requires it. The other backends increase it
  if the `device_stack_size` option is given.
- `asynchronous`: if true, the function may return before the end of the
  computations (see below). An asynchronous evaluation requires
  statistics (status `-5` otherwise).

If no statistics are given, the function returns once the values are
computed, and the output status reports the result.

Otherwise, on all the backends, the output status only reports the errors
of the batch (statuses `-5`, `-6` and `-7`), and the
`S_generic_parallel_report` function gives the result once the
computations are completed, whether the evaluation is asynchronous or not.
If the `asynchronous` option of the policy is true, the backends with a
queue (`CUDA`, `HIP` and `SYCL`) may return once the computations are
submitted to the queue: the calling code synchronizes its queue before
calling the report function. Otherwise, and always with the C++ parallel
algorithms, the function returns once the computations are completed. The statistics
(`mfront_gpmp_Statistics`) are initialized by
`mfront_gpmp_initializeStatistics`. They must be accessible by the host
and by the device where the computations are performed, for instance in
managed or shared memory if the memory space of the library is `device`:
the kernels update them, and the report function reads them on the host
(the C++ parallel algorithms also merge the statuses of the points in
them on the host). With the automatic data
transfers of the `SYCL`
backend, statistics which are not accessible by the device are copied,
as the values, and the evaluation is then synchronous:

~~~~{.cxx}
#include "MFront/GenericParallel/CUDABackendExecutionPolicy.hxx"
// statistics in managed memory
mfront_gpmp_Statistics* statistics = nullptr;
cudaMallocManaged(&statistics, sizeof(mfront_gpmp_Statistics));
mfront_gpmp_initializeStatistics(statistics);
// asynchronous evaluation on a stream
const auto policy = mfront::generic_parallel::makeCUDABackendExecutionPolicy(
    {.common_options = {.asynchronous = true}, .stream = stream});
auto d = mfront_gpmp_BatchData{/* ... */};
d.statistics = statistics;
d.execution_policy = &policy;
// submission of the computations: only the errors of the batch are
// reported here
auto output = mfront_gpmp_OutputStatus{};
Inconel600_YoungModulus_generic_parallel(&output, &d);
// ... other computations submitted to the stream ...
cudaStreamSynchronize(stream);
const auto status =
    Inconel600_YoungModulus_generic_parallel_report(&output, statistics);
~~~~

The report function is only meaningful if the evaluation returned `0`.
An evaluation without statistics calls the same report function on
statistics held by blocks of memory reused from one call to another
(`CUDA`, `HIP` and `SYCL` backends) or by a local variable (C++ parallel
algorithms): the output status is the same.

The statistics give the number of points where the evaluation failed, the number of points where
a bound is violated with the warning policy, and two keys giving the
first point of each kind and the check retained at this point. The keys
are opaque: the minimum of the keys gives the first point, whatever the
order of the computations, and only the report function of the same
library decodes them. A structure of statistics is used for one material
property and one batch. The kernels compute the status of each point and
merge it in the statistics only when an event occurs: with integer atomic
operations (`CUDA`, `HIP` and `SYCL` backends), or by a reduction (C++
parallel algorithms). With the atomic operations, the reporting of the errors costs
nothing when the evaluation succeeds. A reduction combines the statuses of
all the points and copies its result, a few integers, to the host at each
call.

## Output status

The output status (`mfront_gpmp_OutputStatus`) summarizes the evaluation
of the batch. Its `status` member is the error of the batch, if any,
otherwise the status of the first point where the evaluation failed,
otherwise the status of the first point where the result is unreliable,
otherwise `0`. The statuses of the points follow the conventions of the
`generic` interface for material properties:

- `0`: the result has been evaluated at all points;
- `1`: an argument or the result is out of its bounds at some points,
  with the warning policy. The result is evaluated at all points but must
  be used with caution;
- `-1`: an argument or the result is out of its physical bounds, or out
  of its bounds with the strict policy, at some points. The output is
  `NaN` at those points;
- `-4`: the result is not finite (`nan` or infinite) at some points.

The errors of the batch are specific to this interface. Nothing is then
computed:

- `-5`: invalid arguments: no output status (returned value only), no
  data of the batch, no output values, no
  array of arguments or no values for an argument, invalid number of
  arguments, uniform output with several points, more than
  \(2^{48}-1\) points, more blocks of threads than the grid of the device
  can hold with the `CUDA` and `HIP` backends (\(2^{31}-1\) blocks), an
  execution policy of another backend, no execution policy given to the
  `SYCL` backend, an asynchronous evaluation without statistics or with
  an out-of-order `SYCL` queue, or no statistics given to the report
  function;
- `-6`: invalid parameters;
- `-7`: runtime error of the backend: allocation, data transfer,
  submission or execution of a kernel, stack size of the threads of the
  device, or exceptions thrown on the host by the runtimes of the backends
  based on C++ programming models (`SYCL`, C++ parallel algorithms). No
  exception goes through the
  generated functions: an unexpected exception also gives `-7`.

The output status also gives:

- `number_of_failures`: the number of points where the evaluation failed
  (status `-1` or `-4`);
- `first_failure_index`: the index of the first of them, or the maximal
  value of its type. The `generic` interface of the same material
  property, called at this point, gives the detailed error;
- `number_of_unreliable_results`: the number of points where a bound is
  violated with the warning policy;
- `first_unreliable_result_index`: the index of the first of them, or the
  maximal value of its type;
- `bounds_status`: for the statuses `-1` and `1`, the rank of the
  variable of the check retained at the reported point (starting at one,
  the output being after the arguments), negative if the evaluation failed
  (physical bound, or bound with the strict policy), positive otherwise
  (bound with the warning policy), as with the `generic` interface. It is
  null otherwise;
- `msg`: a message describing the reported point (check, index, numbers of
  points). It never gives the values, which the `generic` interface gives
  at this point. Its text is not part of the interface.

The following differences with the `generic` interface are deliberate:
there is no status `-2` (exception) or `-3` (`errno`) at a point (the
kernels do not throw and `errno` does not exist on GPUs), the bounds
status of a point where the result is not finite is null (the `generic`
interface keeps the bounds status of a bound violated with the warning
policy), the message does not give the values, and no `MTest` file is
written when the evaluation fails.

If the runtime checks are disabled (`disable_runtime_checks` DSL option),
as with the `generic` interface, the number of arguments, the parameters,
the bounds and the results are not checked. The checks of the structure
of the batch (pointers, uniform output, number of points, execution
policy) remain: they cost little on the host and avoid invalid
memory accesses. The statuses of the points, if requested, are then null,
and the report function returns `0`.

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
- libGenericParallelUO2-cuda.so :  UO2_ShearModulus_generic_parallel
~~~~

where the configuration file `config-cuda-XXXX.json` provide the
required information to call a `CUDA` compiler (`nvcc` or `clang++`)
with the appropriate flags. This file may also contain the options
associated with the `CUDA` backend, see below. Depending on the parallel
configurations selected during the `cmake` configuration steps, two
configuration files may be installed: `config-cuda-nvcc.json` and
`config-cuda-clang.json`.

The following code shows how to call the function
`UO2_ShearModulus_generic_parallel`. In this example, the porosity is
assumed to be uniform and has thus a stride equal to zero. For the sake of simplicity, memory allocation on
the host and on the device are handled by the [Thrust
library](https://developer.nvidia.com/thrust).

~~~~{.cxx}
thrust::host_vector<double> G;
thrust::host_vector<double> T = {300, 500, 300, 800};
thrust::host_vector<double> f = {0.1};

thrust::device_vector<double> d_T(T);
thrust::device_vector<double> d_f(f);
thrust::device_vector<double> d_G(T.size());

const auto args = std::array<mfront_gpmp_ConstField, 2u>{
    mfront_gpmp_ConstField{.values = thrust::raw_pointer_cast(d_T.data()),
                           .point_stride = 1},
    mfront_gpmp_ConstField{.values = thrust::raw_pointer_cast(d_f.data()),
                           .point_stride = 0}};
const auto d = mfront_gpmp_BatchData{
    .number_of_points = 4,
    .output = mfront_gpmp_Field{.values = thrust::raw_pointer_cast(d_G.data()),
                                .point_stride = 1},
    .arguments = args.data(),
    .number_of_arguments = args.size(),
    .out_of_bounds_policy = GENERIC_MATERIALPROPERTY_NONE_POLICY};
auto output = mfront_gpmp_OutputStatus{};
const auto status = UO2_ShearModulus_generic_parallel(&output, &d);
G = d_G;
~~~~

The following remarks can be made:

- the user is responsible for consistently passing pointers to memory
  allocated on the device to the `UO2_ShearModulus_generic_parallel`
  function, including
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

The `cuda-clang` configuration delivered with `TFEL` uses `-O2` and
`--cuda-gpu-arch=native`, and the `CUDA`
toolkit given by the `TFEL_CLANG_CUDA_PATH` variable of `cmake` (see the
[`TFEL/Config` page](tfel-config.html)): a version of `clang` only
supports the versions of `CUDA` released before it.

### Options

The only option available is `number_of_threads_per_block`, the number
of threads per block. By default, the value used is \(128\) (\(64\) in
the configuration files delivered with `TFEL`). This number is rounded up
at runtime to a multiple of the number of threads per warp, which is
typically \(32\) or \(64\). Typical values are thus \(64\), \(128\)
and \(256\).

### Internal details

When required, the strides are passed by value to the kernel, as kernel
parameters, which `CUDA` stores in constant memory.

If the material property exposes parameters, those are stored in a
global variable on the host (the CPU). For efficiency, the values of the
parameters are also passed by value to the kernel when evaluating the
material property. This mechanism allows to keep the flexibility provided by
parameters: this flexibility is required to perform sensitivity
analyses, uncertainty propagation studies or perform a new
identification. Note that parameters' handling can still be disabled by
setting the `parameters_as_static_variables` DSL option to `true`.

The errors are reported in the statistics of the evaluation, held by managed
memory. Each thread computes the status of its point and only accesses the
statistics, with integer atomic operations, if an event occurs, minimizing
the cost of error handling. Without statistics given by the calling code,
the statistics are taken from a pool of blocks allocated at the first call
on a device and reused by the following calls (see
`mfront::generic_parallel::StatusBlockPool`), so that no memory is allocated
per call. The `HIP` and `SYCL` backends handle the errors in the same way,
with the memory accessible by the host and the device of the backend
(managed memory, shared USM memory). Those blocks are never freed. The C++
parallel algorithms merge the statuses of the points by a reduction.

Before launching a kernel, the stack size of the threads is increased if
the kernel requires it (see the `preserve_stack_size` option of the
execution policies).

On output, the user knows the number of points where the evaluation
failed and the first of them. For instance, the `UO2_ShearModulus`
function may report that the temperature is negative at two points, the
first one being the point 12. For material properties, errors are
associated with violation of bounds and physical bounds, and with
invalid results. Those tests are disabled by setting the
`disable_runtime_checks` DSL option to `true`. Note that this option also
disables other checks performed on the host (notably regarding the
number of arguments passed).

## `HIP` backend

The `HIP` backend is similar to the `CUDA` backend.

### Example of usage

~~~~{.cxx}
auto E = createManagedData(4);
const auto T = createManagedData({300, 500, 300, 800});
const auto args = std::array<mfront_gpmp_ConstField, 1u>{
    mfront_gpmp_ConstField{.values = T.get(), .point_stride = 1}};
const auto d = mfront_gpmp_BatchData{
    .number_of_points = 4,
    .output = mfront_gpmp_Field{.values = E.get(), .point_stride = 1},
    .arguments = args.data(),
    .number_of_arguments = args.size(),
    .out_of_bounds_policy = GENERIC_MATERIALPROPERTY_NONE_POLICY};
auto output = mfront_gpmp_OutputStatus{};
Inconel600_YoungModulus_generic_parallel(&output, &d);
~~~~

where `createManagedData` is a simple wrapper around `hipMallocManaged`
returning an `std::unique_ptr` for RAII.

## Parallel STL backend (`stlpar`)

### Example of usage

Let `UO2_ShearModulus.mfront` be an implementation of a material
property computing the shear modulus of uranium dioxide, which depends
on the temperature and on the porosity. This file can be compiled as
follows:

~~~~{.bash}
$ mfront --obuild --configuration-file=config-stlpar-parunseq-gcc.json \
         --interface=generic-parallel UO2_ShearModulus.mfront
The following library has been built :
- libGenericParallelUO2-stlpar.so :  UO2_ShearModulus_generic_parallel
~~~~

where the configuration file `config-stlpar-parunseq-gcc.json` provide the required
information to compile the generated source files and also selects the
execution policy to be used (see below).

~~~~{.cxx}
auto G = std::vector<double>(4);
const auto T = std::vector<double>{300, 500, 300, 800};
const auto f = std::vector<double>{0.1};
const auto args = std::array<mfront_gpmp_ConstField, 2u>{
    mfront_gpmp_ConstField{.values = T.data(), .point_stride = 1},
    mfront_gpmp_ConstField{.values = f.data(), .point_stride = 0}};
const auto d = mfront_gpmp_BatchData{
    .number_of_points = 4,
    .output = mfront_gpmp_Field{.values = G.data(), .point_stride = 1},
    .arguments = args.data(),
    .number_of_arguments = args.size(),
    .out_of_bounds_policy = GENERIC_MATERIALPROPERTY_NONE_POLICY};
auto output = mfront_gpmp_OutputStatus{};
UO2_ShearModulus_generic_parallel(&output, &d);
~~~~

#### Example of configuration file for `g++`

The following configuration file exemplifies how to use the `g++`
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
  linker_flags : "-ltbb -Wl,-z,nodelete"
}
~~~~

The library is marked as never unloaded (`-Wl,-z,nodelete`): unloading
TBB while its threads are alive crashes the process.

#### Example of configuration file for `nvc++`

The following configuration file exemplifies how to use NVIDIA's `nvc++`
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
    compilation_flags: "-O2 -stdpar=gpu -std=c++20 -march=native" 
  }
},
linking_options : {
  linker_flags: "-stdpar=gpu"
}
~~~~

Those options are the ones of the `stlpar-parunseq-nvhpc-gpu`
configuration.

### Options

Two options are available:

- `execution_policy`, which can have one of the following values:
  - `sequenced_policy` (or `seq`)
  - `unsequenced_policy` (or `unseq`)
  - `parallel_policy` (or `par`)
  - `parallel_unsequenced_policy` (or `par_unseq`, default)
- `device_stack_size`: minimal stack size, in bytes, of the threads of the
  GPU when the computations are offloaded by `nvc++ -stdpar=gpu`.

The exact meaning of those policies are implementations defined and may
depend on compiler flags used. See [this
page](https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag_t)
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
  this library. The configurations linking TBB mark the library as never
  unloaded (`-Wl,-z,nodelete`).

## `SYCL` backend

[`SYCL`](https://khronos.org/sycl) is an open cross-platform abstraction
layer that enables code for heterogeneous and offload processors. `SYCL`
allows mixing code on CPUs (leveraging multithreading), GPUs and
accelerators.

The `SYCL` backend has been tested with the following compilers:
Intel's `icpx` and AdaptiveCpp's `acpp`.

### Example of usage

~~~~{.cxx}
//! \brief type of the memory allocator
using usm_allocator_type =
    sycl::usm_allocator<double, sycl::usm::alloc::host>;
//! \brief a simple alias for a vectors
using usm_vector = std::vector<double, usm_allocator_type>;
//
auto output = mfront_gpmp_OutputStatus{};
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
// execution policy (see MFront/GenericParallel/SyclBackendExecutionPolicy.hxx)
const auto policy = mfront::generic_parallel::makeSyclBackendExecutionPolicy(Q);
// strided argument and output
const auto args = std::array<mfront_gpmp_ConstField, 1u>{
    mfront_gpmp_ConstField{.values = d_T.get(), .point_stride = 2}};
const auto d = mfront_gpmp_BatchData{
    .number_of_points = 4,
    .output = mfront_gpmp_Field{.values = d_E.get(), .point_stride = 3},
    .arguments = args.data(),
    .number_of_arguments = args.size(),
    .out_of_bounds_policy = GENERIC_MATERIALPROPERTY_NONE_POLICY,
    .statuses = nullptr,
    .statistics = nullptr,
    .execution_policy = &policy};
Inconel600_YoungModulus_generic_parallel(&output, &d);
~~~~

### Queue

`SYCL` has no default queue: the queue must be given by the execution
policy (`SyclBackendExecutionPolicy`), which is required. It gives the
device and the context of the unified shared memory of the calling code.
An asynchronous evaluation requires an in-order queue
(`sycl::property::queue::in_order`). With an out-of-order queue, a
synchronous evaluation waits for the computations already submitted to
the queue before the kernel is launched.

### Backend options

This backend has two options:

- `data_transfer_policy` (see below);
- `device_stack_size`: minimal stack size, in bytes, of the threads of an
  NVIDIA GPU used through the `CUDA` runtime of AdaptiveCpp.

#### Data transfer policy

Two policies regarding data transfer between host and device are
available:

- `automatic` (or `auto`, default): the arrays which are not accessible
  by the device are copied in device memory: the arguments, the output,
  the statuses of the points and the statistics. An array is accessible if
  it is allocated by `SYCL` (unified shared memory of any kind: device,
  shared or host) or if the device is the host. The output is copied in
  both directions, so that the values between two values of a strided
  output are kept. The output, the statuses and the statistics are copied
  back once the kernel is completed: the evaluation is then synchronous
  if an array is copied (see `MFront/GenericParallel/SYCLDeviceCopies.hxx`).
- `none`: this policy assumes that the data are accessible by the device.

The errors of the kernels of a synchronous evaluation are delivered to
the asynchronous handler of the queue (`sycl::event::wait_and_throw`):
they are reported with the status `-7` if this handler
throws them, as the handlers of most calling codes do.

> **Providing data on the device**
> 
> Data copy in `SYCL` are asynchronous operations. The generated
> functions submit their commands after the commands previously submitted
> to the given queue: an in-order queue orders them, and the generated
> functions wait for the end of the commands previously submitted to an
> out-of-order queue. If the data are provided by commands submitted to
> another queue, the user is responsible for ensuring that the data are
> effectively copied before calling `MFront`'s generated functions.

# Tests

Unlike other material property interfaces, the tests for the
`generic-parallel` interface are performed in the installation directory.
This allows to test various configurations that may use various `C++`
compilers, a feature unsupported by the `cmake` build system.

The list of configurations tested must be specified by defining a list
named `generic-parallel-configurations` when invoking `cmake` as follows:

~~~~
cmake -Dgeneric-parallel-configurations="cuda-nvcc;cuda-clang"
~~~~

The same list selects the configuration files installed with `TFEL`.

Each configuration runs:

- the tests of the semantics of the generated functions shared by all the
  backends whose arrays are accessible by the host
  (`test-suite/material-properties/generic-parallel/tests.hxx`): statuses
  of the points, failures and unreliable results in the same batch,
  invalid results, errors of the batch, uniform and strided values,
  report function, execution policies (identifier exported by the
  library, policy of another backend, asynchronous evaluation without
  statistics, synchronous evaluation with statistics);
- the tests specific to the backend (execution policies and queues,
  asynchronous evaluations, stack size of the threads of the device,
  memory of the device);
- the comparison of the evaluation on batches of points with the
  evaluation of the `generic` interface at each point (`compare.cxx`):
  values, statuses of the points and summary, for the three out of bounds
  policies, contiguous, strided and uniform values, and synchronous and
  asynchronous evaluations. Both libraries are compiled by the tested
  configuration. The values are compared exactly when the computations
  are performed on the host by a compiler which neither contracts nor
  reorders the floating point operations by default (`g++`, `clang++`,
  `acpp` on the host), and
  up to a relative tolerance of \(10^{-13}\) otherwise (`icpx` and
  computations on a device). The comparisons include a material property with bounds and
  physical bounds on two arguments and on its output
  (`BoundsOrderTest.mfront`), which checks the retained check.

The following tests configurations are currently supported:

- `cuda-nvcc`: compilation of `CUDA` files using the `nvcc` compiler.
- `cuda-clang`: compilation of `CUDA` files using the `clang++`
  compiler.
- `hip-hipcc`: compilation of `HIP` files using the `hipcc` compiler.
- `hip-clang`: compilation of `HIP` files using the `clang++` compiler,
  only on the NVIDIA platform (`HIP_PLATFORM=nvidia`). On the AMD
  platform, this configuration installs no configuration file and
  declares no test, but `HIP` and `clang++` are still required.
- `stlpar-parunseq-gcc`, `stlpar-parunseq-clang`, `stlpar-parunseq-icpx`:
  parallel algorithms of the standard library with the `par_unseq`
  execution policy on the threads of the host (`g++`, `clang++` and `icpx`
  with TBB).
- `stlpar-parunseq-nvhpc-gpu`: parallel algorithms offloaded to an NVIDIA
  GPU by `nvc++ -stdpar=gpu`.
- `sycl-default-icpx`: compilation of `SYCL` files using the `icpx` compiler.
- `sycl-default-acpp`: compilation of `SYCL` files using the `acpp`
  compiler on the CPU.
- `sycl-gpu-acpp`: compilation of `SYCL` files using the `acpp`
  compiler on the GPU using the `generic` target.

### Notes

For configurations based on HIP, the variable `HIP_PLATFORM` must be
passed to `cmake` or defined in the environment. The value of this variable
must be either `nvidia` or `amd`. If `HIP_PLATFORM` is `nvidia`, the
`CUDA` platform must be available. The value of `HIP_PLATFORM` is cached,
so that a reconfiguration does not depend on the environment.
