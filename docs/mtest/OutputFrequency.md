The `OutputFrequency` let the user specify at when results are written
in the output files. This keyword is followed by a string. Two values
are allowed:

- `UserDefinedTimes`: the results are written at each time defined by
  the user. This is the default.
- `EveryPeriod`: the outputs are written after each successful
  period, i.e. at the end of every time step.

This two options differs only in case of substepping.

## Example

~~~~ {.cpp}
@OutputFrequency 'EveryPeriod';
~~~~
