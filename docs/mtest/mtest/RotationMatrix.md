The `@RotationMatrix` let the user specifiy a rotation matrix, so that
the principal directions of the material are different from those used
for the resolution and the boundary conditions.

This keywords accepts an option (given between left and right angles)
named respectively `Standard` (default), `Direction`, `Euler` and
`Miller`. Each option corresponds to a way of defining the rotation
matrix:

- The `Standard` way is to give explicitly the rotation matrix by an
  array of array reprensentig a 3x3 matrix.
- The  `Direction` option let the user specify:
  - a 2D vector giving the first direction of orthotropy in
    modelling hypotheses of space dimension 2.
  - two 3D vectors giving respectively the first and second
    directions of orthotropy under the tridimensional modelling
    hypothesis.
  The given vectors are not required to be normalised. In the 3D
  case, the second vector is not required to be orthogonal to the first
  one. If not, the second direction of orthotropy is deduced from the
  second vector by removing its projection along the first one.
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

~~~~{.cpp}
// standard way
@RotationMatrix {{0,1,0},
                 {1,0,0},
                 {0,0,1}};
~~~~

~~~~{.cpp}
// using direction in 2D
@RotationMatrix<Direction> {0,1};
~~~~

~~~~{.cpp}
// using directions in 3D
@RotationMatrix<Direction> {{0,1,0},{1,0,0}};
~~~~

~~~~{.cpp}
// using miller indices
@RotationMatrix<Miller> {1,5,9};
~~~~
