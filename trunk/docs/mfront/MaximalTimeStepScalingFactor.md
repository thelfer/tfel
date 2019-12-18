The `@MaximalTimeStepScalingFactor` let the user define the default
value of the `maximal_time_step_scaling_factor` parameter.

This parameter defines the maximal value by which the time step will
be increase in case of convergence. This parameter acts as a lower
bound for the value returned by the
`@APosterioriTimeStepScalingFactor` code block.

This keyword is followed by a number which must be strictly greater
than one.

## Note

By default (i.e. if the `@MaximalTimeStepScalingFactor` is not used),
the default value for `maximal_time_step_scaling_factor` parameter
will be very large so that the value returned by the
`@APosterioriTimeStepScalingFactor` code block will not be bounded.

## Example

~~~~ {#MaximalTimeStepScalingFactor .cpp}
@MaximalTimeStepScalingFactor 1.2;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
