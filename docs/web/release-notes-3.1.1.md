% Release notes of the 3.1.1 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2017

This is mainly a bug fix version of the `3.1` series. All tickets
solved are described below.

# New functionalities

## `tfel-config`

### Requesting the C++ standard version used to compile `TFEL`

The `--cxx-standard` option returns the version of the C++ standard
used to compile `TFEL`, as follows:

~~~~{.sh}
$ tfel-config --cxx-standard
17
~~~~

# Tickets fixed

## Ticket #102: The usage of the constants defined in the PhysicalConstants class is broken in debug mode

The various constants defined in the `PhysicalConstants` class are
declared as `constexpr static` variables.

In release mode, those variables are inlined (which is what was
intented). However, in debug mode, those static variables break the
one definition rule of `C++`, leading to undefined symbols in
libraries or executables using them. This is a aknowledged flaw in the
`C++-11` standard which has been solved in `C++-17` which introduces
inline variables.

Thus the code of the `PhysicalConstants` class is only valid in
`C++-17`. In `C++-11`, the solution to this issue in is to remove the
`constexpr` keyword in front of constant declaration and introduce a
library called `TFELPhysicalConstants` in which the constants are
defined.

In `C++-17`, this library is empty.

For more details, see: <https://sourceforge.net/p/tfel/tickets/100/>

## Ticket #100: The name of the parameter for the reference temperature of the geometry is badly choosen

When using the `@ComputeThermalExpansion` keyword, the thermal
expansion is computed as follows:

\[
\frac{\Delta\,l}{l_{T^{i}}} = \frac{1}{1+\alpha\left(T^{i}\right)\,\left(T^{i}-T^{\alpha}\right)}\,\left[\alpha\left(T\right)\,\left(T-T^{\alpha}\right)-\alpha\left(T^{i}\right)\,\left(T^{i}-T^{\alpha}\right)\right]
\]

where:

- \(T^{\alpha}\) is the reference temperature for the thermal
  expansion.
- \(T^{i}\) is the reference temperature at which of the geometry has
  been measured which is generally assumed to be equal to the
  temperature at the beginning of the computations, in the undeformed
  state.

In previous versions of `TFEL` and `MFront`, the reference temperature
for the geometry \(T^{i}\) could be specified as a parameter named
`ReferenceTemperature`. This name was badly choosen and can be
confusing as it is more intuitively associated with \(T^{\alpha}\).

Since this version of `TFEL` and `MFront`, this parameter has been
renamed `ReferenceTemperatureForInitialGeometry`.

For more details, see: <https://sourceforge.net/p/tfel/tickets/100/>

## Ticket #99: Functions defined in `Barlat.ixx` and `Hosford.ixx` does not work for floats

In this case, the functions defined in `Barlat.ixx` and `Hosford.ixx`
can make extensive use of `std::pow(float,int)` when the exponent is
an `int`: this function returns a `double` and not a `float`. Thus the
result of those functions must be recasted in `float`.

For more details, see: <https://sourceforge.net/p/tfel/tickets/99/>

## Ticket #98: Test `calculixelasticity3` shall not be run if the castem interface is not enabled

For more details, see: <https://sourceforge.net/p/tfel/tickets/98/>

# Known incompatibilities

## The name of the parameter used to define the reference temperature of the geometry has changed

The name of the parameter used to define the reference temperature of
the geometry has been changed to
`ReferenceTemperatureForInitialGeometry`. See Ticket #100 for details.

<!-- Local IspellDict: english -->
