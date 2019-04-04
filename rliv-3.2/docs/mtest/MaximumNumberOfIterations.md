The `@MaximumNumberOfIterations` keyword let the user specify the
maximum number of iterations of the global algorithm to reach the
equilibrium.

This keyword is followed by an integer.

If the number of iterations reaches the maximal authorized value, the
time step is divided by two. The maximum number of sub-steps can be
specified using the `@MaximumNumberOfSubSteps` keyword.

## Example

~~~~ {.cpp}
@MaximumNumberOfIterations 10;
~~~~~~~~
