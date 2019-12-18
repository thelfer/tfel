The `@ImposedStrain` keyword allows the user to impose the evolution
of a component of the strains.

This keyword may have one option, which is the way the evolution will
be defined. Four values are accepted: `evolution`, `function`, `data`
or `file`. The `data` and `file` options are equivalent. If no option
is specified, the `evolution` option is choosen.

After the option, the name of the component of the strains, which
shall be given as string, is expected.

Finally, one may optionnally set options on the active state of this
contraint, the activating events and desactivating events using a
JSON-like structure. See the `@Event` keyword to know how to define an
event. This structure starts with an opening curly brace (`{`) and ends
with a closing curly brace (`}`). An option is given by its name, an
double-dot character (`:`) and the value of the option. Consecutive
options are separated by a comma `,` (see below for an example). The
following options are available:

- `active`: states if the constraint is active at the beginning of the
  computation. This is a boolean, so the expected value are either
  `true` or `false`.
- `activating_event`: gives the name of the event which activate the
  constraint. An array of string is expected.
- `activating_events`: gives the list of events which activate the
  constraint. An array of string is expected.
- `desactivating_event`: gives the name of the event which desactivate
  the constraint. An array of string is expected.
- `desactivating_events`: gives the list of events which desactivate the
  constraint. An array of string is expected.

## Name of the components of the strain

According to the modelling hypothesis selected (see the
`@ModellingHypothesis` keyword), the following components names are
accepted:

- AxisymmetricalGeneralisedPlaneStrain : ERR EZZ ETT
- Axisymmetrical                       : ERR EZZ ETT ERZ
- PlaneStress                          : EXX EYY EZZ EXY
- PlaneStrain                          : EXX EYY     EXY
- GeneralisedPlaneStrain               : EXX EYY EZZ EXY
- Tridimensional                       : EXX EYY EZZ EXY EXZ EYZ

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
@ImposedStrain<evolution> 'EXX' 1e-3;
~~~~~~~~

## Example (linear evolution)

~~~~ {.cpp}
@ImposedStrain<evolution> 'EXX' {0.:0.,1:1e-3};
~~~~~~~~

## Example (function)

~~~~ {.cpp}
@ImposedStrain<function> 'EXX' 'e0*sin(t/900.)';
~~~~~~~~

## Example (event)

~~~~ {.cpp}
@ImposedStrain<evolution> 'EXX' {0.:0.,1:1e-3}{
	desactivating_event : 'Stop'
};
~~~~~~~~

## Note

The off-diagonal terms shall be multiplied by a factor \(\sqrt(2)\)
with respect to standard conventions.
