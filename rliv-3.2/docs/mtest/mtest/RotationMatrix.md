The `@RotationMatrix` let the user specifiy a rotation matrix, so that
the principal directions of the material are different from those used
for the resolution and the boundary conditions.

This keywords accepts an option (given between left and right angles)
named respectively `Standard`, `Euler` and `Miller`. Each option
corresponds to a way of defining the rotation matrix:

- the standard and default way is to give explicitly the rotation
  matrix by an array of array reprensentig a 3x3 matrix.
- The `Euler` option allow the user to specify the rotation using
  Euler angles. In this case,  the user must provide an array of
  three angles (expressed in radians).
- The `Miller` option allow the user to specify the rotation using
  miller indices. In this case, the user must provide an array of
  three integers.

### Special case for the Abaqus/Standard and Abaqus/Explicit interface

The `Abaqus/Standard` and `Abaqus/Explicit` interfaces introduce the
notion of othrothropy management policy (see the documentation of
thoses interfaces). In particular, if the behaviour uses the `MFront`
orthotropic management policy, the material orientation has to be
given in the first internal state variables. Those internal state
variables are not accessible by the end user.

`MTest` handles this specific case and extracts the appropriate
information from the rotation matrix given with the `@RotationMatrix`
keywords and copy them to the first internal state variables
automatically.

## Example

~~~~ {.cpp}
// standard way
@RotationMatrix {{0,1,0},
                 {1,0,0},
                 {0,0,1}};
// using miller indices
@RotationMatrix<Miller> {1,5,9};
~~~~~~~~
