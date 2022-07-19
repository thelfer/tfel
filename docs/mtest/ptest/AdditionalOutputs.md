The `@AdditionalOutputs` keyword allow the user to specify additional
outputs in the result file. This keyword is follow by a description of
the outputs using the syntax of a dictionnary where the keys declare the
type of outputs and the values defines the fields to be processed.

The following outputs are available:

- `minimum_value` (or `minimum_values`): this post-processing outputs
  the minimum value of the given field(s).
- `maximum_value` (or `maximum_values`): this post-processing outputs
  the maximum value of the given field(s).
- `integral_value` (or `integral_values`): this post-processing outputs
  the integral value of the given field(s).
- `mean_value` (or `mean_values`): this post-processing outputs the mean
  value of the given field(s), i.e. the integral value divided by the
  volume of the tube.

## Example

~~~~{.cxx}
@AdditionalOutputs{
 "minimum_value" : "BetaPhaseFraction",
 "maximum_values" : {"Temperature","pres","BetaPhaseFraction",
                    "ETT","EquivalentViscoplasticStrain"}
};
~~~~