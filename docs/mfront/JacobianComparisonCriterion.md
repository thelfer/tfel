The `@JacobianComparisonCriterion` keyword defines the criterion value
used for the comparison of the user-defined jacobian to a numerically
computed one. This keyword must be followed by a positive
floating-point number.

### The jacobianComparisonCriterion parameter

The `@JacobianComparisonCriterion` keyword defines the default value
for the `jacobianComparisonCriterion` parameter. This parameter can be
changed at runtime.

### Example

~~~~{.cpp}
@JacobianComparisonCriterion 1.e-6;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


