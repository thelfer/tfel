The global algorithm uses two criteria to check if a satisfying
equilibrium has been found: one on deformation gradient, the other on
stresses.

This criterium on the deformation gradient checks if two successives
estimate of the strains are close enough.  By default, the criterium
value used is 1.e-12;

The `@SDeformationGradient` let the user specifiy the criterium value used
for the criterium on strains.

It is followed by the criterium value.

## Example

~~~~ {.cpp}
@DeformationGradient 1.e-12;
~~~~~~~~
