% Release notes of the 3.0.6 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2019

# Tickets fixed

## Ticket #197: Allow using flags defined in standard environment variables to build `TFEL/MFront`

The variable `USE_EXTERNAL_COMPILER_FLAGS` can now be defined in `cmake`
to use the flags defined in the environment variables `CFLAGS` and
`CXXFLAGS`.

For more details, see: <https://sourceforge.net/p/tfel/tickets/197/>
