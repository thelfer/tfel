In case of failure, three policies can be selected using the
`@FailurePolicy` keyword:

- `ReportOnly`: failure does not affect computation. The evaluation of
  the failure criteria only affects the output file.
- `StopComputation`: failure leads to reject the current time step. If
  substepping is enabled, the time step is divided by two. With this
  policy, one can thus only approach the failure time, but never go
  beyond.
- `FreezeState` (or `FreezeStateUntilEndOfComputation`): if a failure is
  detected, the state of the structure is freezed and do not evolve. No
  equilibrium is performed, the behaviour is no more called and `PipeTest`
  will output the same results again and again until the end of
  computation. This option may be useful when optimizing material
  parameters.

## Example

~~~~{.python}
@FailurePolicy 'FreezeState';
~~~~
