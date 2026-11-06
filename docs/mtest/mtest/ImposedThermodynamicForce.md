The `@ImposedThermodynamicForce` keyword allows the user to impose the
evolution of a component of the thermodynamic forces of the behaviour.

This keyword may have one option, which is the way the evolution will
be defined. Four values are accepted: `evolution`, `function`, `data`
or `file`. The `data` and `file` options are equivalent. If no option
is specified, the `evolution` option is chosen.

After the option, the name of the component of the thermodynamic
forces, which shall be given as string, is expected. A scalar
thermodynamic force is designated by its name. A component of a
tensorial thermodynamic force is designated by the name of the
thermodynamic force followed by the suffix of the component, for
instance `StressXX` for a symmetric tensor named `Stress`. For small
and finite strain behaviours and for cohesive zone models, the names
of the components are the ones given by the `@ImposedStress` and
`@ImposedCohesiveForce` keywords.

The definition of the evolution and the options on the active state of
the constraint are the same as for the `@ImposedStrain` keyword.

This keyword can be used with any behaviour. The `@ImposedStress` and
`@ImposedCohesiveForce` keywords are the specialised versions of this
keyword for small and finite strain behaviours and for cohesive zone
models respectively.

## Example

~~~~ {.cpp}
@ImposedThermodynamicForce 'HydrostaticPressure' {0 : 0, 1 : 1.e6};
~~~~~~~~
