The `@MinimalTimeStepScalingFactor` let the user define the default
value of the `minimal_time_step_scaling_factor` parameter.

This parameter defines the minimal value by which the time step will
be reduce in case of non convergence or when the time step is rejected
by one of the `@APrioriTimeStepScalingFactor` or
`@APosterioriTimeStepScalingFactor` code blocks.

This keyword is followed by a number which must be strictly positive
and strictly lower than one.

## Note

By default (i.e. if the `@MinimalTimeStepScalingFactor` is not used),
the default value for `minimal_time_step_scaling_factor` parameter
will be 0.1.

## Example

~~~~ {#MinimalTimeStepScalingFactor .cpp}
@MinimalTimeStepScalingFactor 0.5;
~~~~
