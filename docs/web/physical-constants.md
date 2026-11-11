---
title: List of physical constants
author: Thomas Helfer
date: 10/11/2026
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tblPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

\newcommand{\mol}{\mathrm{mol}}

The physical constants are extracted from: <http://physics.nist.gov/cuu/Constants/index.htm>

Their values are subject to compatibility modes: the values which are
used by default are those of the compatibility mode of the version,
whereas the values of the next major version, which use all the
published digits of the `CODATA` 2014 values, are available in a
specific namespace.
Compatibility modes are described in the page devoted to
[compatibility modes in the `TFEL` libraries](tfel-compatibility-modes.html).

# Values of the physical constants

Version 5.2 provides the following values, where the `5.2.0` column is
the default:

|   Symbol  |       Description         |     Value (`5.2.0`)          |     Value (`6.0.0`)          |        Unit            |
|-----------+---------------------------+------------------------------+------------------------------+------------------------|
| \(\mu\)   | Atomic mass constant      | \(1.66054\,.10^{-27}\)       | \(1.660539040\,.10^{-27}\)   | \(kg\)                 |
| \(N_{a}\) | Avogadro constant         | \(6.02214\,.10^{23}\)        | \(6.022140857\,.10^{23}\)    | \(\mol^{-1}\)          |
| \(k_{b}\) | Boltzmann constant        | \(1.38065\,.10^{-23} \)      | \(1.38064852\,.10^{-23} \)   | \(J.K^{-1}\)           |
| \(G_{0}\) | Conductance quantum       | \(7.74809\,.10^{-5}\)        | \(7.7480917310\,.10^{-5}\)   | \(S\)                  |
| \(e_{0}\) | Electric constant         | \(8.85419\,.10^{-12}\)       | \(8.854187817\,.10^{-12}\)   | \(F.m^{-1}\)           |
| \(m_{e}\) | Electron mass             | \(9.10938\,.10^{-31} \)      | \(9.10938356\,.10^{-31} \)   | \(kg\)                 |
| \(eV\)    | Electron volt             | \(1.60218\,.10^{-19}\)       | \(1.6021766208\,.10^{-19}\)  | \(J\)                  |
| \(e\)     | Elementary charge         | \(1.60218\,.10^{-19}\)       | \(1.6021766208\,.10^{-19}\)  | \(C\)                  |
| \(F\)     | Faraday constant          | \(96485.3\)                  | \(96485.33289\)              | \(C.\mol^{-1}\)        |
| \(a\)     | Fine-structure constant   | \(7.29735\,.10^{-3}\)        | \(7.2973525664\,.10^{-3}\)   |                        |
| \(R\)     | Molar gas constant        | \(8.31446\)                  | \(8.3144598\)                | \(J.\mol^{-1}.K^{-1}\) |
| \(s\)     | Stefan-Boltzmann constant | \(5.67037\,.10^{-8}\)        | \(5.670367\,.10^{-8}\)       | \(W.m^{-2}.K^{-4}\)    |

The set of the next major version, `6.0.0`, uses the `CODATA` 2014
internationally recommended values with all their published digits.

# Versioned values

The `TFEL/PhysicalConstants` library defines one header per version
which introduced a change of those values, which all declare the same
names:

| Version | Header | `PhysicalConstants` class | Constants |
|---|---|---|---|
| `5.2.0` | `TFEL/PhysicalConstants-5.2.0.hxx` | `tfel::v5_2_0::PhysicalConstants` | `tfel::constants::v5_2_0` |
| `6.0.0` | `TFEL/PhysicalConstants-6.0.0.hxx` | `tfel::v6_0_0::PhysicalConstants` | `tfel::constants::v6_0_0` |

Each of these headers declares:

- a template class `PhysicalConstants<NumericType, use_quantities>`,
  with the same members, in the namespace `tfel::v<x>_<y>_<z>`,
- a variable template per constant, in the namespace
  `tfel::constants::v<x>_<y>_<z>`. Those variable templates have the
  two template parameters `NumericType`, whose default type is
  `double`, and `use_quantities`, whose default value is `false`.

The header `TFEL/PhysicalConstants.hxx` includes all of them and defines
the aliases giving access to the values of the current version, and to
those of the next major version, without naming a mode explicitly:

| Alias | Values |
|---|---|
| `tfel::PhysicalConstants` | those of the current version, `5.2.0` in Version 5.2 |
| `tfel::constants` | those of the current version, `5.2.0` in Version 5.2 |
| `tfel::future::PhysicalConstants` | those of the next major version, `6.0.0` |
| `tfel::constants::future` | those of the next major version, `6.0.0` |

In Version 5.2, `tfel::PhysicalConstants` is
`tfel::v5_2_0::PhysicalConstants`: the unversioned names give the values
of Version 5.1.

# `C++`

The physical constants are available as constexpr static members of
`tfel::PhysicalConstants` class defined in the
`TFEL/PhysicalConstants.hxx` header.

~~~~{.cxx}
using Cste   = tfel::PhysicalConstants<real>;
const auto R = Cste::R;
~~~~

This class gives the values of the current version of `TFEL`. The values
of a given compatibility mode are available in the namespace
`tfel::v<x>_<y>_<z>`, whereas those of the next major version are
available in the namespace `tfel::future`:

~~~~{.cxx}
const auto R = tfel::v5_2_0::PhysicalConstants<real>::R;
const auto s = tfel::future::PhysicalConstants<real>::s;
~~~~

The same values are accessible through the variable templates of the
`tfel::constants` namespace:

~~~~{.cxx}
constexpr auto R = tfel::constants::R<double>;
constexpr auto s = tfel::constants::v6_0_0::s<double>;
~~~~

The following example shows that the default value of the molar gas
constant and the value of the set of the next major version differ, and
how both are accessed:

~~~~{.cxx}
#include <iostream>
#include <iomanip>
#include "TFEL/PhysicalConstants.hxx"

int main() {
  // the values of the current version
  constexpr auto r = tfel::constants::R<double>;
  // a given compatibility mode
  constexpr auto r520 = tfel::constants::v5_2_0::R<double>;
  constexpr auto r600 = tfel::constants::v6_0_0::R<double>;
  // the values of the next major version
  constexpr auto rf = tfel::constants::future::R<double>;
  std::cout << std::setprecision(17);
  std::cout << r << ' ' << r520 << ' ' << r600 << ' ' << rf << '\n';
}  // end of main
~~~~

Since the `TFEL/PhysicalConstants` library is a header-only library,
only the path to the headers is needed to compile this example:

~~~~{.bash}
$ g++ -std=c++20 $(tfel-config --includes) constants.cxx
$ ./a.out
8.3144600000000004 8.3144600000000004 8.3144597999999998 8.3144597999999998
~~~~

The call to `std::setprecision` is required: `std::cout` prints only six
significant digits by default, which is not enough to distinguish the
values of two compatibility modes.

## Quantities

Quantities are used in the same way, by setting the second template
argument to `true`:

~~~~{.cxx}
constexpr auto s = tfel::constants::future::s<double, true>;
~~~~

The class `tfel::PhysicalConstants` and the constants of the current
version can also be used with quantities: `@UseQt` behaviours of
`MFront` do so, as described in the
[quantities section of the `TFEL/Math` documentation](tfel-math.html#sec:tfel_math:quantities).

# `Python`

The physical constants are available as read-only attributes of the
`tfel.PhysicalConstants` class, which gives the values of the current
version of `TFEL`:

~~~~{.python}
>>> from tfel import PhysicalConstants as Cste
>>> print(Cste.Na)
6.02214e+23
~~~~

The values of a given compatibility mode are available in the submodule
of the same name, whereas those of the next major version are available
in the `future` submodule:

~~~~{.python}
>>> import tfel
>>> tfel.PhysicalConstants.R
8.31446
>>> tfel.v5_2_0.PhysicalConstants.R
8.31446
>>> tfel.v6_0_0.PhysicalConstants.R
8.3144598
>>> tfel.future.PhysicalConstants.R
8.3144598
~~~~

# `Fortran`

The `Fortran` bindings, both the `Fortran 77` include file and the
`Fortran 95` module, expose a single set of constants, which is that of
the next major version: they are not affected by compatibility modes at
the time of writing.

# Changing the values of the physical constants

The list of the compatibility modes which affect the values of the
physical constants and those values are those of the
`tools/PhysicalConstants/physical_constants.json` file. A development
which changes the values of the physical constants shall proceed as
follows:

1. Declare the new compatibility mode at the end of the list of the
   compatibility modes, which are the first records of the
   `tools/PhysicalConstants/physical_constants.json` file:

   ~~~~{.json}
   {
     mode: "<X>.<Y>.<Z>";
     description: "Compatibility module with version <X>.<Y>.<Z>.";
   }
   ~~~~

   The compatibility modes are declared in increasing order. The values
   of the last compatibility mode are those of the next major version of
   `TFEL`, which are not used yet. Since the `Python` bindings select
   the values to use with the `TFEL_VERSION_MAJOR` preprocessor variable
   only, the major version of a new compatibility mode shall be greater
   than that of the compatibility mode of the current version of `TFEL`.
2. Give the values of the next major version in the `value` field of the
   constants which change, and those of the compatibility mode those
   values replace in the `values` field of the same records:

   ~~~~{.json}
   {
    name: "StefanBoltzmannConstant";
    short_name: "s";
    description: "Stefan-Boltzmann constant";
    unit: "W.m^{-2}.K^{-4}";
    qt_unit:
      [ 1, 0, -3, 0, -4, 0, 0 ];
    value: 5.670367e-8;
    values:
    [
      {
        mode: "5.2.0";
        value: 5.67037e-8;
      }
    ];
   }
   ~~~~

   A compatibility mode which does not appear in the `values` field uses
   the values of the `value` field. The latter are also those used by the
   language bindings which are not versioned, such as the `Fortran` ones.
   As the values of an already published compatibility mode shall never
   change, the values of the mode of the current version of `TFEL` are
   pinned in the `values` field of each constant whose values change:
   without doing so, changing the values of the next major version would
   change those of the published compatibility mode.
3. Regenerate the headers, the `Python` bindings and the page describing
   the library, then copy them to the source tree:

   ~~~~{.shell}
   g++ physical_constants.cxx -o physical_constants \
       `tfel-config --compiler-flags --includes --libs --utilities`
   ./physical_constants
   clang-format -i PhysicalConstants*.hxx PhysicalConstants-python.cxx
   cp PhysicalConstants-<X>.<Y>.<Z>.hxx PhysicalConstants.hxx \
      ../../include/TFEL/
   cp PhysicalConstants-python.cxx \
      ../../bindings/python/tfel/PhysicalConstants.cxx
   cp TFELPHYSICALCONSTANTS.INC ../../bindings/fortran77/
   cp tfel_physical_constants.f90 ../../bindings/fortran90/
   cp tfel-physical-constants.md ../../docs/web/
   ~~~~

   One header is generated per compatibility mode, together with the
   `PhysicalConstants.hxx` header, which includes all of those headers
   and redirects the names without suffix to the values of the
   compatibility mode of the current version of `TFEL` and those of the
   `tfel::future` and `tfel::constants::future` namespaces to the values
   of the next major version. The `Fortran` bindings expose the values of
   the next major version. Once formatted by `clang-format`, the
   regenerated files are identical to the committed ones, except for the
   values which have changed: checking those differences is the way of
   checking that no value has been changed by mistake.
4. Install the new header in `include/CMakeLists.txt`:

   ~~~~{.cmake}
   install_header(TFEL PhysicalConstants-<X>.<Y>.<Z>.hxx)
   ~~~~

5. Describe the new values in the section giving the
   [values of the physical constants](#values-of-the-physical-constants),
   which tabulates the values of each compatibility mode.
6. Declare the new mode in `MFront`, as described in the
   [corresponding section](mfront-compatibility-modes.html#adding-a-new-compatibility-mode),
   so that `MFront` generates code using the new values.
7. Describe the change in the release notes of the version which
   introduces it and mention the compatibility modes which are
   affected.

The two test models `PhysicalConstants_v5_2_0` and
`PhysicalConstants_v6_0_0` of `MFront`, which return the value of the
Stefan-Boltzmann constant, and the `MTest` files
`physical-constants-5.2.0.mtest` and `physical-constants-6.0.0.mtest`,
which check the value they give, are the reference tests of this
mechanism: a new compatibility mode shall be tested in the same way.

# Known limitations

- The values tabulated in the section giving the
  [values of the physical constants](#values-of-the-physical-constants)
  are described by hand, whereas those of the headers and of the `Python`
  bindings are generated from the
  `tools/PhysicalConstants/physical_constants.json` file: checking that
  those two descriptions agree is done when the values change.
- The `Fortran` bindings are not versioned yet: they expose a single set
  of constants, which is that of the next major version.

# See also

- [Compatibility modes in the `TFEL` libraries](tfel-compatibility-modes.html)
- [Compatibility modes in `MFront`](mfront-compatibility-modes.html)
- [The `TFEL/PhysicalConstants` library](tfel-physical-constants.html)
- [Coding guidelines](coding-guidelines.html#backward-compatibility)
