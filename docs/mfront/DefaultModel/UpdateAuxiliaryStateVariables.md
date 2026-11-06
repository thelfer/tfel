The `@UpdateAuxiliaryStateVariables` introduces a code block meant to
update the auxiliary state variables after integration.

The code declared by `@UpdateAuxiliaryStateVariables` is called once,
at the end of the integration, after the update of the integration
variables (including the state variables). If external point-wise
models were declared (see the `@Model` keyword), the associated
auxiliary state variables are also updated. The external state
variables are not updated: for instance, `T` is the temperature at the
beginning of the time step and `T + dT` the temperature at the end of
the time step.

## Example

~~~~{.cpp}
@UpdateAuxiliaryStateVariables{
  Tmax = max(Tmax, T + dT);
}
~~~~
