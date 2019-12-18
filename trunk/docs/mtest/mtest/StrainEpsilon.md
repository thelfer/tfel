The global algorithm uses two criteria to check if a satisfying
equilibrium has been found: one on strains, the other on stresses.

This criterium on strains checks if two successives estimate of the
strains are close enough.  By default, the criterium value used is
1.e-12;

The `@StrainEpsilon` let the user specifiy the criterium value used
for the criterium on strains.

It is followed by the criterium value.

## Example

~~~~ {.cpp}
@StrainEpsilon 1.e-12;
~~~~~~~~
