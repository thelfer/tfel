% Release notes of the 3.2.3 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2020

Version `3.2.3` is mainly a bug fix version of the `3.2` series.

This version was released along with Version `3.1.5` and inherits from
all the fixes of this version.

# Issues solved

## Ticket #242: The factor in front of the hyperbolic sine is discarded when using the `HyperbolicSine` inelastic flow

The optional factor in in front of the hyperbolic sine was not generated when using the `HyperbolicSine` inelastic flow.

For more details, see: <https://sourceforge.net/p/tfel/tickets/242/>

## Ticket #228: Many stress criteria of the StandardElastoViscoplasticity brick fail when compiling in C++-17

For more details, see: <https://sourceforge.net/p/tfel/tickets/228/>

## Ticket #222: Handling stored and dissated energies is broken in `MTest` for `Abaqus/Standard` behaviours

There were a bug in `MTest` in the `Abaqus/Standard` behaviours which
didn't reinitialize the values of the stored and dissipated energies at
the end of the time step to their values at the beginning of the time
step before the behaviour integration.

Thus, the dissipated and stored energies, when computed incrementally,
are the sums of their increments at each iterations.

For more details, see: <https://sourceforge.net/p/tfel/tickets/222/>

## Ticket #203: Cyrano` interface: compilation process hangs when treating behaviours based on the Hencky strain measure in `AxisymmetricalGeneralizedPlaneStress`

The trouble came from an infinite loop when looking for the external
state variable holding the axial stress.

For more details, see: <https://sourceforge.net/p/tfel/tickets/203/>
