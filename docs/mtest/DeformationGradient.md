The `@DeformationGradient` let the user specifiy the inital value of
the deformation gradient.

This keyword is followed by an array. The size of this array must be
equal to the number of components of (non symmetric) tensors for the
modelling hypothesis considered (see the `@ModellingHypothesis`
keyword).

## Example

~~~~~ {.cpp}
@DeformationGradient {1,1,1,0,0,0,0,0,0};
~~~~~~~~~~
