The global algorithm uses two criteria to check if a satisfying
equilibrium has been found: one on the gradients, the other on the
thermodynamic forces.

The criterion on the thermodynamic forces checks that the residual of
the Newton algorithm is low enough. By default, the criterion value
used is `1.e-3`.

The `@ThermodynamicForceEpsilon` keyword specifies the criterion value
used for the criterion on the thermodynamic forces. It is followed by
the criterion value.

The `@StressEpsilon` and `@CohesiveForceEpsilon` keywords are the
specialised versions of this keyword for small and finite strain
behaviours and for cohesive zone models respectively.

## Example

~~~~ {.cpp}
@ThermodynamicForceEpsilon 1.e-3;
~~~~~~~~
