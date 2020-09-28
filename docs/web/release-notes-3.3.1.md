% Release notes of the 3.3.1 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2020

Version `3.3.1` is mainly a bug fix version of the `3.3` series.

# New features in `MTest`

## Unicode characters in `MTest`

Usage of a limited subsets of `UTF-8` characters in variable names is
now allowed. This subset is described here:

<http://tfel.sourceforge.net/unicode.html>

# Tickets fixed

## Ticket #248: Symbols of time derivative in explicit DSLs are wrong

The symbols associated with the time derivatives of state variables in
explicit DSLs were ill-formed.

For more details, see: <https://sourceforge.net/p/tfel/tickets/243/>

## Ticket #247: Creating array of typed parameters is broken

Creating an array of scalar parameters with a type different than real
was broken.

For more details, see: <https://sourceforge.net/p/tfel/tickets/247/>

## Ticket #243: Generic behaviour interface: throwing an exception in the initialize method is not properly handled

For more details, see: <https://sourceforge.net/p/tfel/tickets/243/>

## Ticket #237: Compilation fails with `intel` compiler 2019 and 2020

For more details, see: <https://sourceforge.net/p/tfel/tickets/237/>

## Ticket #235: Support for `python` bindings in conda

The conda' python interpreter is statically linked, which causes issues
on `Mac Os` if the `python` modules are linked with the `python`
library. See for details
https://github.com/ContinuumIO/anaconda-issues/issues/9078. However, not
linking with the `python` library usually lead to link failures du to
missing symbols and requires the use of specific linker flags, which
causes portability issues.

A proper solution may only appear with CMake 3.15 with the
`Python::module` target, see
https://gitlab.kitware.com/cmake/cmake/issues/18100.

Until this version is widely available, the
`-Dpython-static-interpreter-workaround=ON` can be used at the `cmake`
invokation.

For more details, see: <https://sourceforge.net/p/tfel/tickets/235/>

## Ticket #233: Allow unicode support in variable names in `MTest`

Unicode variables are now correctly handled by the `@Real` and
`@Evolution` keywords.

For more details, see: <https://sourceforge.net/p/tfel/tickets/233/>

## Ticket #232: Better support for the tangent operator blocks in the `generic` interface

The tangent operator blocks are now correctly handled.

For more details, see: <https://sourceforge.net/p/tfel/tickets/232/>

## Ticket #227: Symbolic names are not declared for the tangent operator blocks associated with the derivatives with respect to external state variables

For more details, see: <https://sourceforge.net/p/tfel/tickets/227/>

## Ticket #221: `mfront-query --tangent-operator-blocks` shall return the symbolic name of the tangent operator blocks

For more details, see: <https://sourceforge.net/p/tfel/tickets/221/>

## Ticket #220: Declaring additional tangent operator blocks using symbolic names is broken

For more details, see: <https://sourceforge.net/p/tfel/tickets/220/>

## Ticket #218: The generation of the copy constructor of the `BehaviourData` class is broken when several gradients are defined

For more details, see: <https://sourceforge.net/p/tfel/tickets/218/>

## Ticket #217: Better support of several symmetric tensor gradients

The tangent operator blocks was not defined when several symmetric
tensor gradients were defined.

For more details, see: <https://sourceforge.net/p/tfel/tickets/217/>

## Ticket #216: Computation of the consistent tangent operator in the `mtest::GenericBehaviour` class is broken when several gradients are declared

For more details, see: <https://sourceforge.net/p/tfel/tickets/216/>

## Ticket #215: UTF-8 representations of the tangent operator blocks are not declared

The tangent operator blocks now have proper symbolic representations.

For more details, see: <https://sourceforge.net/p/tfel/tickets/215/>

## Ticket #214: Strings shall not be mangled

Unicode support is based on a mangling used to convert UTF-8 symbols
into ANSYS characters. Strings are no more mangled.

For more details, see: <https://sourceforge.net/p/tfel/tickets/214/>

## Ticket #211: Visual Studio Build Error for DIANAFEA

For more details, see: <https://sourceforge.net/p/tfel/tickets/211/>

## Ticket #198: The `mfront/src/MohrCoulombStressCriterion.cxx` file does not compile in `C++-17`

For more details, see: <https://sourceforge.net/p/tfel/tickets/198/>

