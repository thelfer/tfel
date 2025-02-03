The `@UpdateAuxiliaryStateVariables` introduces a code block meant to
update the auxiliary state variables after integration.

In the `Default` and `Implicit` domain specific languages, the code
declared by `UpdateAuxiliaryStateVariables` is called once the
integration variables (including state variables) and stresses (see the
`@ComputeFinalStress` keyword) have been updated. If external point-wise
models were declared (see the `@Model` keyword), the associated
auxiliary states are also updated. The external state variables are not
updated.

In Runge-Kutta domain specific languages, the code declared by
`UpdateAuxiliaryStateVariables` is called after each successful time
step. Keep in mind that most Runge-Kutta algorithms performs internal
substeppings: in this case, the code declared by
`UpdateAuxiliaryStateVariables` may be called several time during the
behaviour integration. An additional variable called `dt_`, which is
lower than the total time step increment `dt` if substeppings is
performed, gives the current time increment. If external point-wise
models were declared (see the `@Model` keyword), the associated
auxiliary states are set to their values at the current date. The
external state variables are set to their values at the current date.

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

## Example (Runge-Kutta dsl)

~~~~{.cpp}
@UpdateAuxiliaryStateVariables{
  sigeq = sqrt(sig|sig);
}
~~~~
