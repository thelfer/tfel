The `@Times` keywords let the user specify a list of times used for
the computations.

There are currently two ways of specifying this list of times:

- by an array. This is the default.
- by reading the times' values in a file.

The first one is selected by the `array` option. The second one is
selected by one of the equivalent options `data` or `file`. This
option is given between  '`<`' and '`>`' brackets.

## `array` option

In this case, one expects an array describing intervals of times. By
default, one time step is used to go from a given time to the next
one. The keyword 'in' allows the user to divide an interval into a
given number of smaller intervals.

## `data` or `file` option

In this case, one expects a file name followed by the keyword
`using`. The user may then either specify a column number or a
formula as a string. If a formula is given, one may use any constant
or function defined in the input file, such a the ones defined by the
`@Real` keyword. A variable prefixed by `$` followed by a positive
integer holds for the value of a column.

## Note

The loadings and the external state variables evolutions are
described in a totally independent manner (see the `@ImposedStrain`,
the `@ImposedStress` and the `@ExternalStateVariable` keywords).

## Note

In case of non convergence, the algorithm handles time sub
steppings. See the `@MaximumNumberOfSubSteps` for details.

## Example

~~~~ {.cpp}
@Times {0.,3600.};         // 1  time step of  3600. seconds
~~~~~~~~

## Example

~~~~ {.cpp}
@Times {0.,3600. in 10};   // 10 time steps of 360   seconds
~~~~~~~~

## Example

~~~~ {.cpp}
@Times<array> {0.,3600. in 10};   // 10 time steps of 360   seconds
~~~~~~~~

## Example

~~~~ {.cpp}
 // read from file and convert the values read from hours to seconds
@Times<file> 'data.txt' using '$1*3600';
~~~~~~~~

#Example

~~~~ {.cpp}
 // read from file and convert the values read from hours to seconds
@Times<data> 'data.txt' using '$1*3600';
~~~~~~~~
