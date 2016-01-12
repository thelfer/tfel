The `@Evolution` specifies a function of time.

This keyword may have one option, which is the way the evolution will
be defined. Four values are accepted: `evolution`, `function`, `data`
and `file`. The `data` and `file` options are equivalent. If no option
is specified, the `evolution` option is choosen.

After the option, the name of evolution, which shall be given as
string, is expected.

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

## Example (constant evolution):

~~~~ {.cpp}
@Evolution 'Pressure' 1.e5;
~~~~~~~~

## Example (linear evolution):

~~~~ {.cpp}
@Evolution 'Pressure' {0:0.,1.:1.e5};
~~~~~~~~

## Example (function):

~~~~ {.cpp}
@Real P0 2.e5
@Real P1 4.e5
@Evolution<function> 'Pressure' 'P0+(P1-P0)*t**2';
~~~~~~~~

## Example (data):

~~~~ {.cpp}
// times are converted to seconds
// pressure is converted to Pa
@Evolution<data> 'Pressure' using '$1*3600':'$2*1e6';
~~~~~~~~

