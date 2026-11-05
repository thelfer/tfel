The global algorithm uses two criteria to check if a satisfying
equilibrium has been found: one on deformation gradient, the other on
stresses.

This criterion on the deformation gradient checks if two successive
estimate of the strains are close enough.  By default, the criterion
value used is 1.e-12;

The `@DeformationGradientEpsilon` let the user specify the criterion value used
for the criterion on strains.

It is followed by the criterion value.

## Example

~~~~ {.cpp}
@DeformationGradientEpsilon 1.e-12;
~~~~~~~~
