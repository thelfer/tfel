The `@UnitSystem` keyword declares that the state variables, external
state variables an parameters are expressed in a given unit system. In
the current version of `MFront`, the only supported unit system is the
international system of units, denoted `SI`.

One advantage of declaring an unit system is that physical bounds of
variables associated with a glossary entry can automatically be declared
by `MFront`.

For instance the declaration of the physical bounds for the temperature
and the porosity is automatic if the `SI` unit system is used.

## Example of usage

~~~~{.cxx}
@UnitSystem SI;
~~~~
