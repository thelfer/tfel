The `@OpeningDisplacement` keyword specifies the initial value of the
opening displacement of a cohesive zone model.

This keyword is followed by an array. The size of this array must be
equal to the space dimension of the modelling hypothesis considered
(one component per space dimension, see the `@ModellingHypothesis`
keyword).

## Example

~~~~ {.cpp}
@OpeningDisplacement {1.e-4, 0., 0.};
~~~~
