The `@ThermodynamicForce` keyword specifies the initial values of the
thermodynamic forces of the behaviour.

This keyword is followed by an array. The size of this array must be
equal to the total size of the thermodynamic forces of the behaviour
for the modelling hypothesis considered (see the
`@ModellingHypothesis` keyword).

This keyword can be used with any behaviour. The `@Stress` and
`@CohesiveForce` keywords are the specialised versions of this keyword
for small and finite strain behaviours and for cohesive zone models
respectively.

## Example

~~~~ {.cpp}
@ThermodynamicForce {1.e6};
~~~~~~~~
