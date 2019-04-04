The `TangentOperator` keyword introduces a code block used to define
the tangent operator. This code is called once the integration
variables, the stresses and the auxiliary state variables (see the
`@UpdateAuxiliaryStateVariables` keyword) have been updated.

The kind of tangent operator requested is given by variable named
`smt` (stiffness matrix type). As the time of writting this notice,
the possible values for `smt` are the following:

- `ELASTIC`: the elastic operator is requested (undamaged).
- `SECANT`: the secant operator is requested. This operator is only
  meaningful for behaviours describing brittle damage. The expected
  operator is then the elastic operator modified by the damage
  variables.
- `TANGENTOPERATOR`: the tangent operator is requested. This operator
  is seldom implemented as the consistent tangent operator is more
  interesting (and easier to compute).
- `CONSISTENTTANGENTOPERATOR`: the consistent tangent operator is
  requested. If an implicit scheme is used in small strain, the
  consistent tangent operator can be computed using the jacobian of
  the implicit system, see the mfront behaviours documentation for
  details.

## Example

~~~~{.cpp}
@TangentOperator{
  using namespace tfel::material::lame;
  if((smt==ELASTIC)||(smt==SECANTOPERATOR)){
    computeAlteredElasticStiffness<hypothesis,Type>::exe(Dt,lambda,mu);
  } else if (smt==CONSISTENTTANGENTOPERATOR){
    StiffnessTensor De;
    Stensor4 Je;
    computeElasticStiffness<N,Type>::exe(De,lambda,mu);
    getPartialJacobianInvert(Je);
    Dt = De*Je;
  } else {
    return false;
  }
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
