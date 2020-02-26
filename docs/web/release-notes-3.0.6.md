% Release notes of the 3.0.6 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2019

# Tickets fixed

## Ticket #208: Support of the `@GasEquationOfState` keyword is broken in `MTest`

`MTest` is able to model a tight pipe. This hypothesis assumes that the
amount of substance of the gas inside the pipe is constant. The
evolution of the inner pressure is linked to the temperature and radius
evolution through the gas equation of state. By default, the equation of
state of a perfect gas is used.

The `@GasEquationOfState` keyword was introduced to specify the equation
of state of the gas. This feature has never been tested and was not
functional in previous versions.

For more details, see: <https://sourceforge.net/p/tfel/tickets/208/>

## Ticket #207: Circumvent a possible bug in `FindBoost` for `cmake-3.16`

Two boost components are searched when enabling `python` bindings:
`boost-python` and `boost-numpy`. The first one is required and the
second one is optional.

The detection process that was used in previous versions called
`find_package` for two components at the same time and then we checkd of
two components were found.

In `cmake` 3.16 (at least), the behaviour of `find_package` changed and
if one component is missing, both are declared not found.

We now check for `boost-python` first and then search for `boost-numpy`.

For more details, see: <https://sourceforge.net/p/tfel/tickets/207/>

## Ticket #197: Allow using flags defined in standard environment variables to build `TFEL/MFront`

The variable `USE_EXTERNAL_COMPILER_FLAGS` can now be defined in `cmake`
to use the flags defined in the environment variables `CFLAGS` and
`CXXFLAGS`.

For more details, see: <https://sourceforge.net/p/tfel/tickets/197/>
