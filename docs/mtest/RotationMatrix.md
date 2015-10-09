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

## Example

~~~~ {.cpp}
// standard way
@RotationMatrix {{0,1,0},
                 {1,0,0},
                 {0,0,1}};
// using miller indices
@RotationMatrix<Miller> {1,5,9};
~~~~~~~~
