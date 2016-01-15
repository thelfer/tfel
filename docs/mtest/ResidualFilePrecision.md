The `@ResidualFilePrecision` let the user specify the number of
significant digits used when printing residuals at each iteration.

This keyword is followed by the number of digits wanted.

## Note

By default, the precision used for printing the residuals are the same
than the one used for printing results (see the
`@OutputFilePrecision` for details).

## Example

~~~~ {.cpp}
@ResidualFilePrecision 14;
~~~~
