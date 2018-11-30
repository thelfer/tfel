% Release notes of the 3.2.1 version of `TFEL`, `MFront` and `MTest`
% Thomas Helfer
% 2018

This is mainly a bug fix version of the `3.2` series. All tickets
solved are described below.

# Tickets fixed

#Ticket #149: Ability to specify the path the python library at runtime

The following environment variable can now be used to customize how `python` modules for material properties are build:

- `PYTHON_INCLUDE_PATH`: path to the `python` header files.
- `PYTHON_LIBRARY_PATH`: path to the `python` library.
- `PYTHON_LIBRARY`: name of the `python` library.

For more details, see: <https://sourceforge.net/p/tfel/tickets/149/>

#Ticket #142: get energies (elastic, plastic...) in the `generic` interface

Two outputs have been added in the `MFront_GB_State` structure:

- the stored energy, as computed by the `@InternalEnergy` keyword
- the dissipated energy, as computed by the `@DissipatedEnergy` keyword

The `MGIS` project has been updated to match this evolution.

For more details, see: <https://sourceforge.net/p/tfel/tickets/142/>

#Ticket #141: Output `SSE`, `SPD` and `SCD` in `Abaqus/Standard`

The `Abaqus/Standard` solver defines three optional outputs:

- `SSE`: the elastic energy.
- `SPD`: the energy dissipated by plasticity.
- `SCD`: the energy dissipated by viscoplasticity.

On the `MFront` side, the behaviour can optionally compute the stored
and dissipated energies through the `@InternalEnergy` and
`@DissipatedEnergy` keywords. This means that `MFront` behaviours does
not distinguish the energies dissipated by plasticity and
viscoplasticity. The choice made in version `3.2.1` is to store the
energy computed by the `@InternalEnergy` keyword in `SSE` and to store
the energy computed by the `@DissipatedEnergy` keyword in `SPD`.

For more details, see: <https://sourceforge.net/p/tfel/tickets/141/>
