The `@ImposedGradient` keyword allows the user to impose the evolution
of a component of the gradients of the behaviour.

This keyword may have one option, which is the way the evolution will
be defined. Four values are accepted: `evolution`, `function`, `data`
or `file`. The `data` and `file` options are equivalent. If no option
is specified, the `evolution` option is chosen.

After the option, the name of the component of the gradients, which
shall be given as string, is expected. A scalar gradient is designated
by its name. A component of a tensorial gradient is designated by the
name of the gradient followed by the suffix of the component, for
instance `StrainXX` for a symmetric tensor named `Strain`. For small
strain behaviours, finite strain behaviours and cohesive zone models,
the names of the components are the ones given by the
`@ImposedStrain`, `@ImposedDeformationGradient` and
`@ImposedOpeningDisplacement` keywords.

The definition of the evolution and the options on the active state of
the constraint are the same as for the `@ImposedStrain` keyword.

This keyword can be used with any behaviour. The `@ImposedStrain`,
`@ImposedDeformationGradient` and `@ImposedOpeningDisplacement`
keywords are the specialised versions of this keyword for small strain
behaviours, finite strain behaviours and cohesive zone models
respectively. The `@ImposedDrivingVariable` keyword is a synonym of
`@ImposedGradient`.

## Example

~~~~ {.cpp}
@ImposedGradient 'ChangeOfVolume' {0 : 0.13, 1 : 0.24};
~~~~~~~~
