% List of physical constants
% Thomas Helfer
% 2017

\newcommand{\mol}{\mathrm{mol}}

Various physical constants now available. The values are extracted from: <http://physics.nist.gov/cuu/Constants/index.htm>

|   Symbol  |       Description         |          Value               |        Unit            |
|-----------+---------------------------+------------------------------+------------------------|
| \(\mu\)   | Atomic mass constant      | \(1.660539040\,.10^{-27}\)   | \(kg\)                 |
| \(N_{a}\) | Avogadro constant         | \(6.022140857\,.10^{23}\)    | \(\mol^{-1}\)          |
| \(k_{b}\) | Boltzmann constant        | \(1.38064852\,.10^{-23} \)   | \(J.K^{-1}\)           |
| \(G_{0}\) | Conductance quantum       | \(7.7480917310\,.10^{-5}\)   | \(S\)                  |
| \(e_{0}\) | Electric constant         | \(8.854187817\,.10^{-12}\)   | \(F.m^{-1}\)           |
| \(m_{e}\) | Electron mass             | \(9.10938356\,.10^{-31} \)   | \(kg\)                 |
| \(eV\)    | Electron volt             | \(1.6021766208\,.10^{-19}\)  | \(J\)                  |
| \(e\)     | Elementary charge         | \(1.6021766208\,.10^{-19}\)  | \(C\)                  |
| \(F\)     | Faraday constant          | \(96485.33289\)              | \(C.\mol^{-1}\)        |
| \(a\)     | Fine-structure constant   | \(7.2973525664\,.10^{-3}\)   |                        |
| \(R\)     | Molar gas constant        | \(8.3144598\)                | \(J.\mol^{-1}.K^{-1}\) |
| \(s\)     | Stefan-Boltzmann constant | \(5.670367\,.10^{-8}\)       | \(W.m^{-2}.K^{-4}\)    |

# `C++`

The physical constants are available as constexpr static members of
`tfel::PhysicalConstants` class defined in the
`TFEL/PhysicalConstants.hxx` header.

~~~~{.cpp}
using Cste   = tfel::PhysicalConstants<real>;
const auto R = Cste::R;
~~~~

# `Python`

The physical constants are available as members of the
`tfel.PhysicalConstants` class, as follows:

~~~~{.python}
from tfel import PhysicalConstants as Cste
print(Cste.Na)
~~~~
