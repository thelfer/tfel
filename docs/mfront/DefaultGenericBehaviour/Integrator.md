The `@Integrator` keyword allows the user to compute:

- the evolution of the state variables and auxiliary state variables,
- the value of the stress at the end of the time step,
- the consistent tangent operator, if any of the
  @ProvidesSymmetricTangentOperator or @ProvidesTangentOperator was
  used.￼

￼## Specific conventions 
￼

￼The following conventions apply: 

- for any thermodynamic force thf (declared using the
  @ThermodynamicForce keyword), thf represents the value of the
  thermodynamic force at the beginning of the time step. This value must
  be updated.
- for any gradient g (declared using the @Gradient keyword), g
  represents the value of the gradient the beginning of the time step
  and dg represents its increment (constant over the time step).
- for any internal variable Y (declared using the @StateVariable
  keyword), Y represents its value at the beginning of the time step.
  The increment of Y, represented by the variable dY, must be computed.
- for any auxiliary internal variable Y (declared using the
  @AuxiliaryStateVariable keyword), Y represents its value at the
  beginning of the time step. This value must be updated.
- for any external variable V (declared using the @ExternalStateVariable
  keyword), V represents its value at the beginning of the time step and
  dV represents its increment over the time step (constant over the time
  step).

If the consistent tangent operator is to be computed, the variable Dt is
available. The computeTangentOperator_ boolean value states if the the
computation of the tangent operator has been requested by the calling
solver. The smt variable (stiffness matrix type) contains the kind of
tangent operator requested.


## Notes

The difference between a state variable and an auxiliary state variable
lies in the fact that the increment is defined for the state variable.
This increment is used to update the state variable **after** the
execution of the @Integrator code block.

By default, the temperature T is automatically declared as the first
external state variable.

The consistent tangent operator can also be computed in the
@TangentOperator code block. However, computing the consistent tangent
operator in the @Integrator code block is often more convenient.
