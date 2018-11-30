% Release notes of the 3.1.4 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2018

This is mainly a bug fix version of the `3.1` series. All tickets
solved are described below.

# Tickets fixed

## Ticket #148: Library suffix is not taken into account by the `cmake` generator

For more details, see: <https://sourceforge.net/p/tfel/tickets/148/>

## Ticket #146: `ExternalLibraryManager::hasBounds` is broken

For more details, see: <https://sourceforge.net/p/tfel/tickets/146/>

## Ticket #144: Issue in the generic plane stress handler of the `Cast3M` interface

The generic plane stress handler provided by the `Cast3M` interface has
been designed to add support to the plane stress hypothesis when the
behaviour does not support it "natively".

The interface creates a wrapper around the behaviour which is then
integrated in generalised plane stress, the axial strain being an
additional unknown.

In TFEL-3.0`, we tried to mimic more closely the behaviour of standard
behaviour by requiring the `PlateWidth` (`DIM3`) material property to be
defined. This implies to reorder the material properties when calling
the behaviour in generalised plane strain. This was done correctly for
orthotropic behaviours, but not for isotropic behaviours.

This case was tested by `MTest` but we did not reproduce correctly the
`Cast3M` behaviour.

For more details, see: <https://sourceforge.net/p/tfel/tickets/144/>

## Ticket #143: Wrong detection of the ability to compute the disspated energy  in the `Abaqus/Explicit` interface

The dissipated energy can be computed using the `@DissipatedEnergy` keyword. As only a few interface support this feature (in TFEL 3.1, only the `Abaqus/Explicit` does) and that most behaviours does not provide the computation of the dissipated energy, we rely on tag dispatching to perform the computation, if available. 

In `mfront/include/MFront/Abaqus/AbaqusExplicitInterface.hxx`, the following code was wrong:

~~~~{.cpp}
static constexpr const bool bi = MTraits::hasComputeInternalEnergy;
static constexpr const bool bd = MTraits::hasComputeDissipatedEnergy;
....
using DEnergyComputer = typename std::conditional<bi,DissipatedEnergyComputer,
						      DoNothingEnergyComputer>::type;
~~~~

We used `bi` to make the dispatch in lieu of `bd`. The corrected version is the following:

~~~~{.cpp}
using DEnergyComputer = typename std::conditional<bi,DissipatedEnergyComputer,
						      DoNothingEnergyComputer>::type;
~~~~

For more details, see: <https://sourceforge.net/p/tfel/tickets/143/>
