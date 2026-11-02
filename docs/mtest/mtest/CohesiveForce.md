The `@CohesiveForce` keyword let the user specifiy the inital value of
the cohesive forces.

This keyword is followed by an array. The size of this array must be
equal to the space dimension of the modelling hypothesis considered
(one component per space dimension, see the `@ModellingHypothesis`
keyword).

## Example

~~~~ {.cpp}
@CohesiveForce {'T0', 0., 0.};
~~~~
