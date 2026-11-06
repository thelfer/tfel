The global algorithm uses two criteria to check if a satisfying
equilibrium has been found: one on strains, the other on stresses.

This criterion on stresses checks that the residual of the Newton
Algorithm is low enough. By default, this value is equal to 1.e-3,
which is a convenient value if stresses are expressed in Pa.

The `@StressEpsilon` keyword let the user specify the criterion value
used for the criterion on stresses.

It is followed by the criterion value.

## Example

~~~~ {.cpp}
@StressEpsilon 1.e2;
~~~~~~~~
