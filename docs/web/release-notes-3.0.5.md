% Release notes of the 3.0.5 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2019

This is mainly a bug fix version of the `3.0` series. All tickets
solved are described below.

# Tickets fixed

## Ticket #189: "return false" doesn't work with some Runge-Kutta integrators

The issue concerns all the rk algorithms without local time-step
control (`euler`, `rk2`, `rk4`) which are not used in practice.

For more details, see: <https://sourceforge.net/p/tfel/tickets/180/>
