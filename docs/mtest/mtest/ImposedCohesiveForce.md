The `@ImposedCohesiveForce` keyword allows the user to impose the
evolution of a component of the cohesive force of a cohesive zone
model.

This keyword may have one option, which is the way the evolution will
be defined. Four values are accepted: `evolution`, `function`, `data`
or `file`. The `data` and `file` options are equivalent. If no option
is specified, the `evolution` option is chosen.

After the option, the name of the component of the cohesive force,
which shall be given as string, is expected.

The definition of the evolution and the options on the active state of
the constraint are the same as for the `@ImposedStrain` keyword.

## Name of the components of the cohesive force

According to the modelling hypothesis selected (see the
`@ModellingHypothesis` keyword), the following components names are
accepted:

- PlaneStress                          : Tn Tt
- PlaneStrain                          : Tn Tt
- GeneralisedPlaneStrain               : Tn Tt
- Tridimensional                       : Tn Tt1 Tt2

`Tn` is the normal component and `Tt`, `Tt1` and `Tt2` are the
tangential components.

## Example

~~~~ {.cpp}
@ImposedCohesiveForce 'Tn' {0 : 0, 1 : 1.e6};
~~~~~~~~
