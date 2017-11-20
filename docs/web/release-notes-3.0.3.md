% Release notes of the 3.0.3 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2017

This is mainly a bug fix version of the `3.0` series. All tickets
solved are described below.

# Tickets fixed

## Ticket #96: Building with the `musl` `c` library

The `Alpine` `Linux` distribution is based on the `musl` `c` library
and not `glibc`. Thus, the `feenableexcept` and `fedisableexcept`
functions are not available.

Appropriate tests, based on the definition of the `__GLIBC__` flags
were added to enable the compilation of `TFEL/MFront` with the `musl`
`c` library.

For more details, see: <https://sourceforge.net/p/tfel/tickets/96/>

## Ticket #94: The `pymfronttest1` test shall not be executed if the `Aster` interface is not enabled

For more details, see: <https://sourceforge.net/p/tfel/tickets/94/>
