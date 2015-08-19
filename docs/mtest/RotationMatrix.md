The `@RotationMatrix` let the user specifiy a rotation matrix, so that
the principal directions of the material are different from those used
for the resolution and the boundary conditions.

This keyword is followed by an array of array reprensentig a 3x3
matrix.

## Example

~~~~ {.cpp}
@RotationMatrix {{0,1,0},
                 {1,0,0},
                 {0,0,1}};
~~~~~~~~
