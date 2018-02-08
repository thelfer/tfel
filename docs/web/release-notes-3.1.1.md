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

## Ticket #111: Have a flag stating if the implicit system is called for the evaluation of the jacobian by perturbation

In some cases, it is convenient to update auxiliary state variables
in the `@Integrator` code block, rather than computing them in
the `@UpdateAuxiliaryStateVariables` code block which is only called
once the convergence is reached.

However, if the jacobian matrix is computed numerically (at least
partially), such updates could be wrong, because they can be based
on the perturbated values of the unknowns. 

In `TFEL 3.1`, this can be circumvented by testing the value of the
`perturbatedSystemEvaluation` boolean value as follows:

~~~~{.cpp}
// let av be an auxiliary state variable
@AuxiliaryStateVariable StrainStensor av;

@Integrator{
  // put updated value of av in a temporary variable
  const auto av_ = eval(...);  
  ...
  definition of the implicit system
  ...
  if(!perturbatedSystemEvaluation){
    // update auxiliary state variables
    av = av_;
  }
} // end of @Integrator
~~~~

In many cases, rather than updating auxiliary variables during the
Newton iterations, it can be more pratical to compute its increment,
defined in by local variable and to update the auxiliary variable in
the `@UpdateAuxiliaryStateVariables` code block. The previous trick can
be used in this case in a straightforward manner.

For more details, see: <https://sourceforge.net/p/tfel/tickets/111/>

## Ticket #110: Numerically computed parts of the jacobian shall not be updated after convergence

In previous versions, the numerically computed parts of the jacobian
were updated after the convergence has been reached to ensure that
a proper jacobian is available for the computation of the consistent
tangent operator.

In `TFEL-3.1.1`, those parts of the jacobian are no more updated after
convergence if the consistent tangent operator is not requested.

For more details, see: <https://sourceforge.net/p/tfel/tickets/110/>

## Ticket #108: Imcompatibility between `@StrainMeasure Hencky` and `@ModellingHypothesis AxisymmetricalGeneralisedPlaneStrain`

This is a mistake in the `Cast3M` interface which happens when:

- using the `MieheApelLambrecht` finite strain strategy or using the
  Hencky strain measure (this is equivalent for the `Cast3M`
  interface)
- the plane stress hypothesis is not directly supported by the
  behaviour. In this case, the `Cast3M` interface tries to use the
  `GeneralisedPlaneStrain` hypothesis to support plane stress
  computations.

Because of some ill-formed test in `CastemInterface.cxx`, the
interface tried to access to the `BehaviourData` associated with the
`GeneralisedPlaneStrain` hypothesis ... which may be not
available. Hence the error.

For more details, see: <https://sourceforge.net/p/tfel/tickets/108/>

## Ticket #107: The ZMAT interface does not support array of parameters

For more details, see: <https://sourceforge.net/p/tfel/tickets/107/>

## Ticket #106: Compilation of MFront 3.1 fails for Intel Compiler

Compilation of MFront 3.1 fails for Intel Compiler (version
icc/2016.0.047) with this message:

~~~~{.bash}
mfm/src/mfm.cxx(232): error: invalid type conversion: "const tfel::utilities::Argument" to "const std::string &" const auto& l = static_cast<const std::string&="">(a);
~~~~

This problem is limited to this version of the Intel Compiler. The
solution is to change this line with:

~~~~{.cpp}
const auto& l = a.as_string();
~~~~

For more details, see: <https://sourceforge.net/p/tfel/tickets/106/>

## Ticket #105: Getting version number of `MFront`

In order to check the MFront usage with `CYRANO3`, its would be usefull
to retrieve the version of `MFront`:

- used to generate a given shared library.
- associated with the library: `libTFELMFront.so` which is dynamically
  loaded in `CYRANO3`.

### Metadata in `MFront` generated libraries

A specific symbol is now generated for every generated entry points
(functions or classes) defined in `MFront` libraries.

This symbol' name is composed by the entry point name followed by
`_tfel_version`.

For example, if we are testing the version used to generated the
`cyranonorton` function, one have to retrieve the
`cyranonorton_tfel_version` symbol.

This can be done:

- directly using `dlopen/dlsym`.
- by using the `getTFELVersion` method of the `ExternalLibraryManager`
  class.

The `tfel_version` member has been added to the
`ExternalBehaviourDescription` class which is used by `Cyrano`. Beware
that this can lead to inconsistencies if a previous version of `TFEL`
is used.

### Retrieving the `TFEL` version at runtime

The `TFEL` version can be retrived by calling the `getTFELVersion`
function defined in the `TFELConfig` shared library.

For more details, see: <https://sourceforge.net/p/tfel/tickets/105/>

## Ticket #104: Checks of material properties' bounds are not generated anymore 

The checks for material properties are not generated any more. This is
due to an ill-formed test in `BehaviourDSLCommon.cxx` in the
`writeMaterialPropertyCheckBoundsEvaluation` method of the
`BehaviourDSLCommon` class.

For more details, see: <https://sourceforge.net/p/tfel/tickets/104/>

## Ticket #103: The temperature is updated twice in updateExternalStateVariables


The temperature is updated twice in
`updateExternalStateVariables`. This is due to the fact that the
temperature has been explicitly integrated in the external state
variables in the 3.1 release (in previous releases, the temperature
was treated independently, which led to lots of code duplication and
special cases).

This bug affects behaviours relying on local sub-stepping, a feature
provided by the `Cast3M` and `Cyrano` interfaces. The other interfaces
are not affected.

For more details, see: <https://sourceforge.net/p/tfel/tickets/103/>

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
