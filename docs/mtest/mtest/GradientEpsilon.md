The global algorithm uses two criteria to check if a satisfying
equilibrium has been found: one on the gradients, the other on the
thermodynamic forces.

The criterion on the gradients checks if two successive estimates of
the gradients are close enough. By default, the criterion value used
is `1.e-12`.

The `@GradientEpsilon` keyword specifies the criterion value used for
the criterion on the gradients. It is followed by the criterion value.

The `@StrainEpsilon`, `@DeformationGradientEpsilon` and
`@OpeningDisplacementEpsilon` keywords are the specialised versions of
this keyword for small strain behaviours, finite strain behaviours and
cohesive zone models respectively. The `@DrivingVariableEpsilon`
keyword is a synonym of `@GradientEpsilon`.

## Example

~~~~ {.cpp}
@GradientEpsilon 1.e-10;
~~~~~~~~
