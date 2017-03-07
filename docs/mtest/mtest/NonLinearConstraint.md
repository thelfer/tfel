The `@NonLinearConstraint` keyword let the user introduce arbitrary
constraints on driving variables and thermodynamic forces.

> **Note**
>
> This keyword can also be used to define linear constraints, although
> the numerical treatment of such a constraint will be sub-optimal. A
> special treatment of such a constraint is planned.

## Normalisation policy

This keyword must be followed by an option giving the normalisation
policy. The normalisation policy can have one of the following values:

- `DrivingVariable`, `Strain`, `DeformationGradient`,
  `OpeningDisplacement` stating that the constraint is of the order of
  magnitude of the driving variable.
- `ThermodynamicForce`, `Stress`, `CohesiveForce` stating that the
  constraint is of the order of magnitude of the thermodynamic force.

## Example

~~~~~ {.cpp}
// ensure that the loading is isochoric in 1D
@NonLinearEquation<Strain> 'FRR*FTT*FZZ-1';
~~~~~~~~~~
