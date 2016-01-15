The `@MaximalTimeStepScalingFactor` keyword let the user define the
maximal time step scaling factor allowed during automatic time step
adaptation (see `@DynamicTimeStepScaling` for details).

This keyword is followed by a value. This value must be greater than
one.

## Example

~~~~ {.cpp}
@MaximalTimeStepScalingFactor 1.2;
~~~~
