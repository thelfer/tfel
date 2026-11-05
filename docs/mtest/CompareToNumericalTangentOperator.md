If the `@CompareToNumericalTangentOperator` is followed by `true`, a
comparison of the tangent operator given by the behaviour to a
numerical approximation computed by a centered finite difference is
performed.

The value of the perturbation used to compute this numerical
approximation can be adjusted by the
`@NumericalTangentOperatorPerturbationValue`.

The value used for to compare the tangent operator given by the behaviour to a
numerical approximation can be adjusted by the
`@TangentOperatorComparisonCriterion`.

This comparison is a debugging tool: when it fails, the tangent
operator given by the behaviour and its numerical approximation are
printed in the output of `MTest`, but the test is not considered as
failed.

## Example

~~~~ {.cpp}
@CompareToNumericalTangentOperator true;
~~~~~~~~
