The `@ImposedDeformationGradient` keyword allows the user to impose
the evolution of a component of the deformation gradient.

This keyword may have one option, which is the way the evolution will
be defined. Two values are accepted: `evolution` and `function`. If no
option is specified, the `evolution` option is choosen.

After the option, the name of the component of the deformation
gradient, which shall be given as string, is expected.

According to the modelling hypothesis selected (see the
`@ModellingHypothesis` keyword), the following components names are
accepted:

- AxisymmetricalGeneralisedPlaneStrain : FRR FZZ FTT
- Axisymmetrical                       : FRR FZZ FTT ERZ FZR
- PlaneStress                          : FXX FYY FZZ FXY FYX
- PlaneStrain                          : FXX FYY     FXY FYX
- GeneralisedPlaneStrain               : FXX FYY FZZ FXY FYX
- Tridimensional                       : FXX FYY FZZ FXY FYX FXZ FZX FYZ FZY

If the `evolution` option has been selected, the user may specify a
constant evolution by simply giving its value. Otherwise, complex
evolutions can be build using a associative array where the key is the
time and the value the value of the evolution. Between two times, the
values will be interpolated linearly. Prior the first declared time
given, the value corresponding the this first time is used. After the
last time given, the value corresponding this last time is used.

If the `function` option has been selected, a string is expected which
is interpreted as a function of time. The current time is represented
by the variable `t`.

## Example (constant evolution)

~~~~ {.cpp}
@ImposedDeformationGradient<evolution> 'FXX' 1e-3;
~~~~~~~~

## Example (linear evolution)

~~~~ {.cpp}
@ImposedDeformationGradient<evolution> 'FXX' {0.:0.,1:1e-3};
~~~~~~~~

## Example (function)

~~~~ {.cpp}
@ImposedDeformationGradient<function> 'FXX' 'e0*sin(t/900.)';
~~~~~~~~
