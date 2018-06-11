% Release notes of the 3.1.3 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2018

This is mainly a bug fix version of the `3.1` series. All tickets
solved are described below.

# Tickets fixed

## Ticket #134: The `MFRONT_COMPILING` flag is not defined when using the `CMake` generator

This bug leads to the failure of the compilation of `MFront` files based
on other `MFront` files (such as material properties) on `Windows` with
the `Visual` `C++` compiler.

For more details, see: <https://sourceforge.net/p/tfel/tickets/134/>
