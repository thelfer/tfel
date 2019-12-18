The `@AxialGrowthEvolution` keyword allows the user to impose the
evolution of the axial growth.

This keyword may have one option, which is the way the evolution will
be defined. Two values are accepted: `evolution`, `function`, `data`
or `file`. The `data` and `file` options are equivalent. If no option
is specified, the `evolution` option is chosen.

## `evolution` option

If the `evolution` option has been selected, the user may specify a
constant evolution by simply giving its value. Otherwise, complex
evolutions can be build using a associative array where the key is the
time and the value the value of the evolution. Between two times, the
values will be interpolated linearly. Prior the first declared time
given, the value corresponding the this first time is used. After the
last time given, the value corresponding this last time is used.

## `function` option

If the `function` option has been selected, a string is expected which
wil be interpreted as a function of time. The time is represented by
the variable `t`.

## `data` or `file` options

With the `data` or `file` options, one may define an evolution based
on the values defined in a file. Those values will be interpolated
linearly.

After the evolution name, one expects a file name as a string. This
file name must be followed by:

- the `using` keyword
- a column number or a formula defining the times
- the `:` character
- a column number or a formula defining the values

This syntax has been choosen to mimic the `gnuplot` one.

A formula can use any constant or function defined in the input file,
such a the ones defined by the `@Real` keyword. A variable prefixed by
the `$` followed by a positive integer holds for the value of a
column.

## Example (constant evolution)

~~~~ {.cpp}
@AxialGrowthEvolution 1.e-2;
~~~~~~~~

## Example (linear evolution)

~~~~ {.cpp}
@AxialGrowthEvolution<evolution> {0.:0.,1:1.e-2};
~~~~~~~~

## Example (function)

~~~~ {.cpp}
@AxialGrowthEvolution<function>  '1.e-2*sin(t/900.)';
~~~~~~~~
