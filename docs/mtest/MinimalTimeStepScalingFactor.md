The `@MinimalTimeStepScalingFactor` keyword let the user define the
minimal time step scaling factor allowed during automatic time step
adaptation (see `@DynamicTimeStepScaling` for details).

This keyword is followed by a value. This value must be strictly
positive and lower than one.

## Example

~~~~ {.cpp}
@MinimalTimeStepScalingFactor 0.2;
~~~~
