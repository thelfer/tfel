% Release notes of the 3.1.1 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2017

This is mainly a bug fix version of the `3.1` series. All tickets
solved are described below.

# Tickets fixed


## Ticket #99: Functions defined in `Barlat.ixx` and `Hosford.ixx` does not work for floats

In this case, the functions defined in `Barlat.ixx` and `Hosford.ixx`
can make extensive use of `std::pow(float,int)` when the exponent is
an `int`: this function returns a `double` and not a `float`. Thus the
result of those functions must be recasted in `float`.

For more details, see: <https://sourceforge.net/p/tfel/tickets/99/>

## Ticket #98: Test `calculixelasticity3` shall not be run if the castem interface is not enabled

For more details, see: <https://sourceforge.net/p/tfel/tickets/98/>

