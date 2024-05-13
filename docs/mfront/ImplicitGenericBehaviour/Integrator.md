The `@Integrator` code block allows the user to construct a residual,
and, depending on the algorihm used (selected by the @Algorithm
keyword), its derivative, called the jacobian in the following which
aims at determining the increments of the integration variables
(integration variables are introduced either by the @IntegrationVariable
or the @StateVariable keyword).

￼## Specific conventions 
￼

￼The following conventions apply: 

- for any thermodynamic force thf (declared using the
  @ThermodynamicForce keyword), thf represents the value of the
  thermodynamic force at the the middle of the time step (i.e.
  at t + theta * dt) calculated using the @ComputeThermodynamicForce
  code block.
- for any gradient g (declared using the @Gradient keyword), g
  represents the value of the gradient the beginning of the time step
  and dg represents its increment (constant over the time step).
- for any internal variable Y (introduced by the @StateVariable), Y
  represents its value at the start of the time step;
- for any integration variable (introduced either by the
  @IntegrationVariable keyword or by the @StateVariable keyword) Y, dY
  represents the current estimate of the increment of this variable over
  the time step.
- for any auxiliary internal variable Y (declared by the
  @AuxiliaryStateVariable keyword), Y represents its value at the
  beginning of the time step.
- for any external variable V (declared by the @ExternalStateVariable
  keyword), V represents its value at the beginning of the time step and
  dV represents its increment over the time step (constant over the time
  step).

fY is the part of the residual associated  to the integration variable Y.

In the jacobian matrix is needed, dfY1_ddY2 is the block of the jacobian
matrix corresponding to the derivative of fY1 (part of the residual
associated with Y1) with respect to dY2 (increment of the Y2 integration
variable).

## Notes

- For an integration variable Y declared by the @IntegrationVariable
  keyword (and not introduced by the @StateVariable keyword), the
  variable Y is defined by not initialized.
- The residual can be accessed through the fzeros variable.
- The jacobian matrix can be accessed through the jacobian variable.
