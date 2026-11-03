The `@ImposedOpeningDisplacement` keyword allows the user to impose the
evolution of a component of the opening displacement of a cohesive
zone model.

This keyword may have one option, which is the way the evolution will
be defined. Four values are accepted: `evolution`, `function`, `data`
or `file`. The `data` and `file` options are equivalent. If no option
is specified, the `evolution` option is chosen.

After the option, the name of the component of the opening
displacement, which shall be given as string, is expected.

The definition of the evolution and the options on the active state of
the constraint are the same as for the `@ImposedStrain` keyword.

## Name of the components of the opening displacement

According to the modelling hypothesis selected (see the
`@ModellingHypothesis` keyword), the following components names are
accepted:

- PlaneStress                          : Un Ut
- PlaneStrain                          : Un Ut
- GeneralisedPlaneStrain               : Un Ut
- Tridimensional                       : Un Ut1 Ut2

`Un` is the normal component and `Ut`, `Ut1` and `Ut2` are the
tangential components.

## Example

~~~~ {.cpp}
@ImposedOpeningDisplacement 'Un' {0 : 0, 1 : 1.e-4};
~~~~~~~~
