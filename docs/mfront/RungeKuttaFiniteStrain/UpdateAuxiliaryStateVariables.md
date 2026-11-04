The `@UpdateAuxiliaryStateVariables` introduces a code block meant to
update the auxiliary state variables after integration.

The code declared by `@UpdateAuxiliaryStateVariables` is called at the
end of each successful step of the Runge-Kutta algorithm, after the
update of the state variables and of the stress. Keep in mind that
most Runge-Kutta algorithms perform internal substepping: in this
case, the code declared by `@UpdateAuxiliaryStateVariables` may be
called several times during the behaviour integration. An additional
variable called `dt_`, which is lower than the total time step
increment `dt` if substepping is performed, gives the current time
increment.

The deformation gradient `F`, the external state variables and the
auxiliary state variables computed by external point-wise models (see
the `@Model` keyword) are set to their values at the current date: for
instance, `T` is the temperature at the current date.

## Example

~~~~{.cpp}
@UpdateAuxiliaryStateVariables{
  sigeq = sqrt(sig|sig);
}
~~~~
