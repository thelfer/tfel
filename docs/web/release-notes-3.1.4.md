% Release notes of the 3.1.4 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2018

This is mainly a bug fix version of the `3.1` series. All tickets
solved are described below.

# Tickets fixed

## Ticket 143: Wrong detection of the ability to compute the disspated energy  in the `Abaqus/Explicit` interface

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
