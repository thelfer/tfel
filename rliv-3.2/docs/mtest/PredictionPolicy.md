The `@PredictionPolicy` keyword let the user how the initial estimate
of the solution shall be obtained.

This keyword is followed by a string. The following values are
allowed:

- NoPrediction
- LinearPrediction
- ElasticPrediction
- ElasticPredictionFromMaterialProperties
- SecantPrediction
- TangentPrediction

## Example

~~~~ {.cpp}
@PredictionPolicy 'ElasticPrediction';
~~~~~~~~

## Note

The `ElasticPredictionFromMaterialProperties` policy is only supported
by the `umat` interface as the `Cast3M` finite element solver requires
the stiffness matrix to be defined.

