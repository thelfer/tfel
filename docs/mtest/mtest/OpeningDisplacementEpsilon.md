The global algorithm uses two criteria to check if a satisfying
equilibrium has been found: one on opening displacements, the other on
cohesive forces.

This criterion on opening displacements checks if two successive
estimates of the opening displacements are close enough. By default,
the criterion value used is 1.e-12.

The `@OpeningDisplacementEpsilon` keyword specifies the criterion value
used for the criterion on opening displacements.

It is followed by the criterion value.

## Example

~~~~ {.cpp}
@OpeningDisplacementEpsilon 1.e-12;
~~~~~~~~
