% Installing `TFEL/MFront` on `FreeBSD`
% Thomas Helfer
% 5/11/2017

This page covers some specific issues arising when compiling
`TFEL/MFront` on `FreeBSD`.

The recommended compiler on `FreeBSD` is `clang`.

# `python` bindings

The `python` bindings shall work as expected. Beware to use a
version of the `boost` python library 

## `FreeBSD 11.1` and `boost` version `1.65.1`

On `FreeBSD 11.1`, `boost` version `1.65.1` has a bug that prevents
the `init` function of modules from being exported.  See
https://github.com/boostorg/python/issues/173 for details.

You can workaround this issue by setting the following flag when
 invoking `cmake`:
 `-Denable-broken-boost-python-module-visibility-handling=ON`.
