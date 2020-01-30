% Release notes of the 3.2.3 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2020

Version `3.2.3` is mainly a bug fix version of the `3.2` series.

This version was released along with Version `3.1.5` and inherits from
all the fixes of this version.

# Issues solved

## Ticket #203: Cyrano` interface: compilation process hangs when treating behaviours based on the Hencky strain measure in `AxisymmetricalGeneralizedPlaneStress`

The trouble came from an infinite loop when looking for the external
state variable holding the axial stress.

For more details, see: <https://sourceforge.net/p/tfel/tickets/203/>




