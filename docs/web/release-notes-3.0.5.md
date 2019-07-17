% Release notes of the 3.0.5 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2019

This is mainly a bug fix version of the `3.0` series. All tickets
solved are described below.

# Tickets fixed

## Ticket #181: Runge-Kutta DSL does not handle `NaN` values

For most algorithms with correction-prediction, the Runge-Kutta DSL has
two strategies to evaluate the error. The first one (the default) is the
euclidian norm and the second one the based on the absolute norm. The
DSL switches from one strategy to the other depending on the size of
unknowns.

The trouble only concerns the second strategy.

For more details, see: <https://sourceforge.net/p/tfel/tickets/181/>

## Ticket #180: "return false" doesn't work with some Runge-Kutta integrators

The issue concerns all the rk algorithms without local time-step
control (`euler`, `rk2`, `rk4`) which are not used in practice.

For more details, see: <https://sourceforge.net/p/tfel/tickets/180/>
