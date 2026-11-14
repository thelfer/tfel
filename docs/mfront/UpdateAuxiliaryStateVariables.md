The `@UpdateAuxiliaryStateVariables` introduces a code block meant to
update the auxiliary state variables after integration.

The code declared by `@UpdateAuxiliaryStateVariables` is called once,
at the end of the behaviour integration, after the update of the
integration variables (including the state variables) and of the
thermodynamic forces (the stress for instance). If external point-wise
models were declared (see the `@Model` keyword), the associated
auxiliary state variables are also updated. The gradients and the
external state variables are not updated: for instance, `T` is the
temperature at the beginning of the time step and `T + dT` the
temperature at the end of the time step.

## Example (Implicit dsl)

~~~~{.cpp}
@UpdateAuxiliaryStateVariables{
  // valeur de milieu de pas, q ayant
  // déjà été mis à jour
  const real q_ = q-(1-theta)*dq;
  const real Q = Q0 + (Qm - Q0) * (1 - exp(-2 * Mu * q_) );
  R+=b*(Q-R)*dp;
}
~~~~
