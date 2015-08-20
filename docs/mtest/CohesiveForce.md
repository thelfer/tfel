The `@CohesiveForce` let the user specifiy the inital value of the
cohesive forces.

This keyword is followed by an array. The size of this array must be
equal to the number of components of symmetric tensors for the
modelling hypothesis considered (see the `@ModellingHypothesis`
keyword).

## Example

~~~~ {.cpp}
@CohesiveForce {'T0,,0.,0.};
~~~~
