---
title: List of physical constants
author: Thomas Helfer
date: 2017
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
whereas the values of the next major version, which use all the digits
of the `double` representation, are available in a specific namespace.
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

Compatibility modes are described in the page devoted to
[compatibility modes in the `TFEL` libraries](tfel-compatibility-modes.html).

# `C++`

The physical constants are available as constexpr static members of
`tfel::PhysicalConstants` class defined in the
`TFEL/PhysicalConstants.hxx` header.

~~~~{.cpp}
using Cste   = tfel::PhysicalConstants<real>;
const auto R = Cste::R;
~~~~

This class gives the values of the current version of `TFEL`. The values
of a given compatibility mode are available in the namespace
`tfel::v<x>_<y>_<z>`, whereas those of the next major version are
available in the namespace `tfel::future`:

~~~~{.cpp}
const auto R = tfel::v5_2_0::PhysicalConstants<real>::R;
const auto s = tfel::future::PhysicalConstants<real>::s;
~~~~

# `Python`

The physical constants are available as members of the
`tfel.PhysicalConstants` class, as follows:

~~~~{.python}
from tfel import PhysicalConstants as Cste
print(Cste.Na)
~~~~

This class gives the values of the current version of `TFEL`. The values
of a given compatibility mode are available in the submodule of the same
name:

~~~~{.python}
import tfel
print(tfel.v5_2_0.PhysicalConstants.Na)
print(tfel.future.PhysicalConstants.Na)
~~~~

# `Fortran`

The `Fortran` bindings expose a single set of constants, which is not
affected by compatibility modes at the time of writing.
