The `@Evolution` specifies a function of time.

This keyword may have one option, which is the way the evolution will
be defined. Two values are accepted: `evolution` and `function`. If no
option is specified, the `evolution` option is choosen.

After the option, the name of evolution, which shall be given as
string, is expected.

If the `evolution` option has been selected, the user may specify a
constant evolution by simply giving its value. Otherwise, complex
evolutions can be build using a associative array where the key is the
time and the value the value of the evolution. Between two times, the
values will be interpolated linearly. Prior the first declared time
given, the value corresponding the this first time is used. After the
last time given, the value corresponding this last time is used.

If the `function` option has been selected, a string is expected which
wil be interpreted as a function of time. The time is represented by
the variable `t`.

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

