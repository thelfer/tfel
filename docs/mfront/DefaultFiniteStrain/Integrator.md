The `@Integrator` keyword allows the user to compute:

- the evolution of the state variables and auxiliary state variables,
- the value of the stress at the end of the time step.

￼## Specific conventions 
￼

￼The following conventions apply: 

- sig is initialized by value of the Cauchy stress at the beginning of
  the time step. This value must be updated.
- F0 represents the deformation gradient at the beginning of the time
  step and F1 represents the deformation gradient at the end of the time
  step.
- for any internal variable Y, Y represents its value at the start of
  the time step. The increment of Y, represented by the variable dY,
  must be computed.
- for any auxiliary internal variable Y, Y represents its value at the
  start of the time step. This value must be updated.
- for any external variable V, V represents its value at the start of
  the time step and dV represents its increment over the time step
  (constant over the time step).

## Notes

The difference between a state variable and an auxiliary state variable
lies in the fact that the increment is defined for the state variable.
This increment is used to update the state variable **after** the
execution of the @Integrator code block.

By default, the temperature T is automatically declared as the first
external state variable.
