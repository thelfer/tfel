The `@CompareToNumericalJacobian` keyword enables or disables the
comparison of the user defined jacobian to a numerically computed
jacobian. This keyword is followed by a boolean value (`true` or
`false`).

Jacobians are compared by blocks. Blocks for which the comparison
failed are reported on the standard output.

The comparison criterion value can be changed used the
`@JacobianComparisonCriterion` keyword.

### Example

~~~~{.cpp}
@CompareToNumericalJacobian true;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
