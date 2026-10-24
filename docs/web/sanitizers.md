---
title: Running sanitized tests
author: Thomas Helfer
date: 24/10/2026
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

This page describes how to build `TFEL/MFront` with sanitizers and how to
run the tests of the build tree with them. The available `cmake` options are
described in the [installation documentation](install.html#sanitizers-and-hardening-options).

## Choosing the sanitizers

A sanitizer is enabled either by listing it in the `enable-sanitize-options`
option or by enabling its dedicated option, `enable-<name>-sanitizer`:

~~~~ {#sanitize-address .bash}
$ cmake <srcdir> -B build -DCMAKE_BUILD_TYPE=Debug \
      -Denable-sanitize-options="address,undefined"
~~~~

A comma may be used as a separator instead of a semicolon. The available
sanitizers depend on the compiler and on its version: the dedicated option
of an unavailable sanitizer is simply ignored with a warning, so that the
same options can be used whatever the compiler is. The `address`, `thread`
and `memory` sanitizers cannot be used simultaneously.

The combination which gives the best return for `TFEL/MFront` is the address
sanitizer combined with the undefined behaviour sanitizer and its subsets:

- With `gcc`, we recommend listing the checks of the undefined behaviour
  sanitizer individually rather than using `undefined`, as the `vptr` check,
  which is part of `undefined`, may lead to pathologically long compilation
  times for the heavy template code of `MFront` (see the release notes):

  ~~~~ {#sanitize-gcc .bash}
  $ cmake <srcdir> -B build -DCMAKE_BUILD_TYPE=Debug \
        -Denable-sanitize-options="address,alignment,bool,bounds,bounds-strict,builtin,enum,float-cast-overflow,float-divide-by-zero,null,object-size,return,shift,shift-base,shift-exponent,signed-integer-overflow" \
        -Denable-glibcxx-debug=ON
  ~~~~

- With `clang`, the `undefined` sanitizer can be used directly:

  ~~~~ {#sanitize-clang .bash}
  $ CC=clang CXX=clang++ cmake <srcdir> -B build -DCMAKE_BUILD_TYPE=Debug \
        -Denable-sanitize-options="address,undefined" \
        -Denable-glibcxx-debug=ON
  ~~~~

Note that `clang` requires the runtimes of the sanitizers (part of
`compiler-rt`, often packaged as `libclang-rt-<version>-dev` or
`libclang-common-<version>-dev`) to be installed: if the sanitizer flags are
rejected at link time, the corresponding sanitizer is disabled by `cmake`
with a `sanitizer '<name>' disabled` message, and no sanitizer flag is used.

## Standard library hardening

The `enable-glibcxx-debug` option adds the `-D_GLIBCXX_DEBUG` definition in
debug builds, which enables the debug mode of the standard library of the
GNU C++ library (bounds checking of the containers, valid iterator checks,
etc.). It can be combined with the sanitizers above.

With `clang` using the `libc++` standard library
(`enable-libcxx=ON`), the `enable-libcxx-hardening-mode` option enables the
hardening mode of `libc++` instead.

## Building and running the tests

The usual steps of the continuous integration, with sanitizers, are:

~~~~ {#sanitize-build-tests .bash}
$ cmake --build build --config Debug
$ cmake --build build --config Debug --target build-tests
$ ctest --test-dir build --build-config Debug
~~~~

The behaviour of the sanitizers at execution time is controlled by
environment variables. We recommend the following settings to get full
reports:

~~~~ {#sanitize-options .bash}
$ ASAN_OPTIONS=detect_leaks=1 UBSAN_OPTIONS=print_stacktrace=1 \
  ctest --test-dir build --build-config Debug
~~~~

- `ASAN_OPTIONS=detect_leaks=1` enables the leak sanitizer with the address
  sanitizer (it is not enabled by default with `clang`).
- `UBSAN_OPTIONS=print_stacktrace=1` makes the undefined behaviour sanitizer
  print a stack trace, which is much more useful than a source location
  alone.

A sanitizer report makes the tested program exit with a non zero status, and
thus the corresponding test fails: any sanitizer report is reported by
`ctest` as a test failure.

## Known pitfalls

### Installed TFEL shadowing the libraries of the build tree

With `-Denable-glibcxx-debug=ON`, the executables of the build tree must use
the libraries of the build tree: the `_GLIBCXX_DEBUG` mode changes the
symbols of the functions using standard containers
(`std::__debug::vector`, etc.) and mixing hardened and non hardened code
gives `undefined symbol` errors at startup.

This is a frequent issue for `TFEL` developers, because `LD_LIBRARY_PATH`
(`TFELHOME`, etc.) of the development environment points to an installed,
not hardened, `TFEL`, and `LD_LIBRARY_PATH` has precedence over the
`RUNPATH` of the executables. In such a situation, the tests fail
immediately (in a fraction of a second) with messages such as:

~~~~ {#sanitize-symbol-error .bash}
symbol lookup error: ...: undefined symbol: ...NSt7__debug6vector...
~~~~

To avoid this, run the tests in an environment free of the installed `TFEL`:

~~~~ {#sanitize-clean-env .bash}
$ unset TFELHOME TFELPLOTHOME TFELEDITORHOME
$ export LD_LIBRARY_PATH=$(echo "$LD_LIBRARY_PATH" | tr ':' '\n' | grep -v tfel | paste -sd:)
$ export PATH=$(echo "$PATH" | tr ':' '\n' | grep -v tfel | paste -sd:)
$ ctest --test-dir build --build-config Debug
~~~~

(the continuous integration machines have a clean environment, so this issue
does not show up there).

### Resources

- A debug build with sanitizers is large (the build tree including the tests
  is about 20 GB): check that the filesystems hosting the build tree and the
  compiler temporary files (directory used by `TMPDIR`) have enough space.
  A full filesystem appears as compilation errors of the form `error
  writing to /tmp/ccXXXXXX.s: No space left on device`.
- Compiling the heaviest test files of `TFEL` (heavy template
  instantiations with unit types) with sanitizers requires several GB of
  memory per compilation process: on a 15 GB machine, building the tests
  with more than four parallel jobs may exhaust the memory, which appears as
  `fatal error: ... signal terminated program cc1plus` errors.

## Going further

- The `gcc` and `clang` documentations describe many more sanitizers: they
  can be declared in `cmake/modules/gcc.cmake` and
  `cmake/modules/clang.cmake` in the same way as the already declared ones.
- See also the result of the [Coverity Scan](coverity-scan.html) static
  analysis, which is complementary to the sanitizers.
