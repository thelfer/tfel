The `@ComputeStress` keyword introduces a code block meant to compute
the stress symmetric tensor.

This keyword interprets the code block to generate two methods:

- The first one is used before the integration step, using updated
  values for the state variables and external state variables.
- The second one is a candidate for the computation of the stress at
  the end of the integration. This candidate is used if the user does
  not provide an appropriate way of computing the stress at the end of
  the time step using the `@ComputeFinalStress` keyword.

## Note

If the user provide a way of computing the stress at the end of the
time step through the `@ComputeFinalStress` keyword, we consider that
the use of `@ComputeStress` is meaningless and advice the user to
rather compute explicitly the stress as part of the integration step.

## Example

~~~~{.cpp}
@ComputeStress{
  sig = (1-d)*(lambda*trace(eel)*Stensor::Id()+2*mu*eel);
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

