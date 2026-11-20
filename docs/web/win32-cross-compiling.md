---
title: Cross-compiling for Windows on a LiNuX host
author: Thomas Helfer
date: 20/11/2026
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

`TFEL` can be cross-compiled for `Windows` using `MinGW` from a `LiNuX`
host. Such a build is used to build the `Windows` installers (see the
[dedicated page](packages.html)). Since version 5.2, the tests of such a build
can also be run on the host, using `wine`.

## Tool chain files

The following tool chain files are provided:

| File | Compiler | Comments |
| ---- | -------- | -------- |
| `cmake/ToolChain-x86_64-w64-mingw32.cmake` | `x86_64-w64-mingw32-g++` | 64 bits |
| `cmake/ToolChain-i686-w64-mingw32.cmake` | `i686-w64-mingw32-g++` | 32 bits |
| `cmake/ToolChain-i586-mingw32msvc.cmake` | `i586-mingw32msvc-g++` | obsolete tool chain, only supported for historical reasons |

All of them set `CMAKE_SYSTEM_NAME` to `Windows`, the three front ends (`C`,
`C++`, and `fortran`), the target environment (`CMAKE_FIND_ROOT_PATH`), and
adjust `CMAKE_FIND_ROOT_PATH_MODE_*` so that programs are searched on the host
whereas headers and libraries are searched in the target environment. They also
define a `<compiler>_COMPILER` variable, which is used to disable the features
that the corresponding tool chain does not support, and the
`TFEL_MISSING_STD_THREADS_SUPPORT` preprocessor definition (see
[Limitations](#limitations)).

On `Ubuntu` or `Debian`, the compilers of the `mingw-w64` packages are used:

~~~~ {#install_mingw .bash}
$ sudo apt install g++-mingw-w64-x86-64 gfortran-mingw-w64-x86-64
~~~~

The version of the compiler matters: version 5.x of `TFEL` requires a compiler
supporting [`std::bit_cast`](https://github.com/thelfer/tfel/issues/918), which
is `gcc` 11 or later, whereas the `mingw-w64` packages of `Ubuntu` 22.04 are
based on `gcc` 10.

## Configuring a cross-compiled build

A cross-compiled build is configured as any other build, using the
`CMAKE_TOOLCHAIN_FILE` option:

~~~~ {#configure_cross_compiled_build .bash}
$ mkdir build-cross-x86_64 && cd build-cross-x86_64
$ cmake <sources> -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_TOOLCHAIN_FILE=<sources>/cmake/ToolChain-x86_64-w64-mingw32.cmake \
      -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-portable-build=ON
~~~~

The `enable-static` option has to be used when a `Windows` installer is built,
since the runtime libraries of the cross compiler would not be present on the
target machine (this is what is done to build the `NSIS` packages, see the
[dedicated page](packages.html)).

## Running the tests of a cross-compiled build

When cross-compiling with a `MinGW` tool chain, `wine` is searched on the host,
and the following message tells if the tests can be run or not:

~~~~ {#wine_detection .bash}
-- wine found: the tests of the cross-compiled build can be run
~~~~

The programs of the build tree are run through a launcher generated at the top
of the build directory, `wine-launcher.sh`, which is used as
`CMAKE_CROSSCOMPILING_EMULATOR`. The tests are then run in the usual way, either
with `make check` (which builds them first), or with `ctest` after the `tests`
target has been built:

~~~~ {#run_cross_compiled_tests .bash}
$ make -j8
$ make -j8 build-tests
$ ctest -j8
~~~~

### Why a launcher is needed

A program built in the build tree only finds the shared libraries it is linked
to, either because those are installed in a directory searched by default, or
because the environment variable `PATH` has been set accordingly. Native
`Windows` builds generate the required environment files (`env.bat`, `env.ps1`,
…) for the second case, and the tests set the `PATH` variable through the
`ENVIRONMENT` property of the tests.

This approach cannot be used with `wine`, for two reasons:

- `wine` does not inherit the environment of the host: the value of `PATH` set by
  `ctest` is simply not visible from the program run through `wine`. Configuring
  the `Path` value of the `HKCU\Environment` key of the registry of a prefix is
  not a way around this limitation, as the values which are stored this way are
  either ignored (if they use `/` as a separator), or not taken into account (if
  they use `\`).
- the commands of the tests are not always run by `ctest` itself: some tests run
  a program of the build tree using `execute_process` from a `cmake` script, so
  the `CROSSCOMPILING_EMULATOR` variable is ignored since `cmake` is the host's
  one. The same happens for the code generation performed at build time, as
  `add_custom_command` does not honour `CROSSCOMPILING_EMULATOR` either.

### How the launcher works

The launcher uses the only directory which is always searched by `wine` for the
shared libraries of a program: the system directory of the prefix,
`<prefix>/drive_c/windows` (indeed, `wine` considers `windows` and
`windows/system32` as being the same directory).

The launcher copies in this directory:

- all the `dll`s of the build tree, which are searched with `find`;
- the runtime libraries of the cross compiler (`libstdc++-6.dll`,
  `libgcc_s_*.dll`, `libwinpthread-1.dll`, `libgfortran-*.dll`, …), whose
  location is obtained from the compilers themselves using
  `-print-file-name=<library>`, as those libraries are not installed next to them
  on `Ubuntu` for example.

The prefix used is `<build directory>/wine-prefix`, so that the prefix of the
user, usually `~/.wine`, is neither used, nor modified, by the tests. It is
created the first time the launcher is run, in a few seconds, using

~~~~ {#create_wine_prefix .bash}
$ WINEDLLOVERRIDES="mscoree,mshtml=" wineboot -i
~~~~

so that no connection to the network is attempted, and no installation prompt
blocks the build. Two environment variables are always set by the launcher:
`WINEPREFIX` to use this prefix, and `WINEDEBUG=-all` to avoid polluting the
output of the tests with the messages of `wine`.

Copying about two hundred libraries takes about a second, which would be
unacceptable for each of the twenty thousands tests. The copies are therefore
only refreshed when a shared library of the build tree is newer than a stamp
file, which is not the case during a test phase. Several processes may ask for
the libraries to be refreshed at the same time, as the code generation performed
by `mfront` is run in parallel: a lock is therefore used, and a lock left by a
killed process is discarded after two minutes.

The exit status of the program is propagated, so that a test which fails is
reported as a failure by `ctest`. In particular, a program which cannot be run
because a shared library is missing exits with the status 53
(`STATUS_DLL_NOT_FOUND`), which `ctest` reports as a failure: a test cannot
pass silently because it was not actually run.

### Diagnosing a failure

Setting `WINEDEBUG` to another value gives the details of what `wine` does. In
particular, the following command shows why a shared library cannot be found:

~~~~ {#diagnose_wine .bash}
$ WINEDEBUG=err+module <build directory>/wine-launcher.sh <program> <arguments>
0024:err:module:import_dll Library libTFELMFront.dll not found
~~~~

Note that a program which does not exist is reported the same way, and that the
status is then 53 too.

## Using `check_all.sh`

The `-w` option of `check_all.sh` cross-compiles `TFEL` with each of the `MinGW`
compilers which are available on the host, and runs the corresponding tests if
`wine` is available:

~~~~ {#check_all_w .bash}
$ ./check_all.sh -f -w -b <build directory> <sources>
~~~~

The first cross-compilation of a day takes a few hours, as the source
distribution (`-s` option) is also tested. It can be skipped using the `-p`
(`--portable-build`) option and by reducing the number of parallel jobs.

## Limitations

- The `win32` threading model of `MinGW` does not provide `std::thread`,
  `std::mutex`, …, which is the case of the `mingw-w64` packages of `Ubuntu`
  22.04. The tool chain files then define `TFEL_MISSING_STD_THREADS_SUPPORT`,
  which excludes `ThreadPool` and its tests from `TFELSystem`, and makes the
  `Abaqus` examples of the documentation use a `CreateMutex` based replacement.
  A cross compiler using the `posix` threading model, as those provided by
  `MSYS2` or the `MinGW` build of `_cast3m_`, supports `std::thread`: the tests
  of `ThreadPool` can then be run by removing this definition from the tool chain
  file.

- Version 5.x of `TFEL` requires `gcc` 11 or later, which the `mingw-w64`
  packages of `Ubuntu` 22.04 do not provide (`gcc` 10). Two unit tests cannot be
  built: `StringAlgorithms` (uses `std::bit_cast`) and
  `GaussKronrodQuadratureTest` (type punning in a constant expression).

- Only the 64 bits tool chain can be tested if the 32 bits version of `wine` is
  not installed, which is the case on `Ubuntu` unless the `wine32:i386` package
  has been installed. The 32 bits binaries are simply not run.

- The tests comparing the output of a program with a reference file cannot be run
  on the host with a program built for `Windows`, as the output of the latter
  uses the `\r\n` line endings whereas the reference files always use the Unix
  ones (as stated by the `.gitattributes` file). The line endings of the outputs
  are therefore normalized before the comparison.

- Very few tests fail from time to time when `ctest` is run with a high level of
  parallelism, and always succeed when run alone. This is not specific to a
  cross-compiled build, but running the tests with a lower number of jobs, e.g.
  `ctest -j8` rather than `ctest -j16`, reduces the issue.

- The test `mfront-CompatibilityModeTest` cannot be linked, as reported in
  [issue #1476](https://github.com/thelfer/tfel/issues/1476).

## 

<!-- Local IspellDict: english -->
