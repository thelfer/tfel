Failure criteria can be added to pipe modelling using the
`@FailureCriterion` keyword.

A failure criterion is called at the end of each time step to detect
failure of the pipe.

Each failure criterion adds a column to the output file giving the
status of the criterion:

- `0` means that the criterion is not met, i.e. no failure is detected
  and the pipe is sound.
- `1` means that the criterion is met, i.e. failure is detected and the
  pipe is broken.

See the `@FailurePolicy` keyword for selecting how a failure shall be
handled by `mtest`.

## Example

~~~~{.python}
@FailureCriterion 'ElongationAtBreak' {maximum_value : 1e-4};
~~~~
