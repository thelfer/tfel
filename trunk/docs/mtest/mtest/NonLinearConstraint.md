The `@NonLinearConstraint` keyword let the user introduce arbitrary
constraints on driving variables and thermodynamic forces.

> **Note**
>
> This keyword can also be used to define linear constraints, although
> the numerical treatment of such a constraint will be sub-optimal. A
> special treatment of such a constraint is planned.

> **Note**
>
> A stress component whose value is imposed using `@ImposedStress`
> command shall not appear in a non linear constraint: the numerical
> treatment of those two constraints is incompatible.

## Applications

Abritray non linear constraints can be used to:

- Impose a constant stress triaxiality.
- Impose a constant first Piola-Kirchoff stress in a creep test
  described in finite strain.

## On the physical meaning of a constraint

A constraint \(c\) is imposed by introducing a Lagrange multiplier
\(\lambda\).

Consider a small strain elastic behaviour characterised by its free
energy \(\Psi\). In the only loading is the constraint \(c\), the
solution satisfies:
\[ \underset{\underline{\varepsilon},\lambda}{\min}\Psi-\lambda\,c \]

In this case, the constraint \(c\) is equivalent to the following
imposed stress:

\[
-\lambda\,\deriv{c}{\underline{\varepsilon}}
\]

If the constraint is \(\sigma_{xx}-\sigma_{0}\), where \(\sigma_{0}\)
is a constant value, the previous equation shows that imposing this
constraint *is not equivalent* to imposing an uniaxial stress state
\(\left(\sigma_{xx}\,0\,0\,0\,0\,0\right)\).

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
@NonLinearConstraint<Strain> 'FRR*FTT*FZZ-1';
~~~~~~~~~~

~~~~~ {.cpp}
// impose the first piola kirchoff stress
// in an uniaxial compression test
@Real 'Pi0' -40e6
@NonLinearConstraint<Stress> 'SXX*FYY*FZZ-Pi0';
@NonLinearConstraint<Stress> 'SYY';
@NonLinearConstraint<Stress> 'SZZ';
@NonLinearConstraint<Stress> 'SXY';
@NonLinearConstraint<Stress> 'SXZ';
@NonLinearConstraint<Stress> 'SYZ';
~~~~~~~~~~
