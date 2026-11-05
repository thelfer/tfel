The global algorithm uses two criteria to check if a satisfying
equilibrium has been found: one on opening displacements, the other on
cohesive forces.

This criterion on cohesive forces checks that the residual of the
Newton algorithm is low enough. By default, this value is equal to
1.e-3, which is a convenient value if cohesive force are expressed in
Newton.

The `@CohesiveForceEpsilon` let the user specify the criterion value used
for the criterion on cohesive force.

It is followed by the criterion value.

## Example

~~~~~ {.cpp}
@CohesiveForceEpsilon 1.e-3;
~~~~~~~~~~
