The `@Gradient` keyword specifies the initial values of the gradients
of the behaviour.

This keyword is followed by an array. The size of this array must be
equal to the total size of the gradients of the behaviour for the
modelling hypothesis considered (see the `@ModellingHypothesis`
keyword).

This keyword can be used with any behaviour. The `@Strain`,
`@DeformationGradient` and `@OpeningDisplacement` keywords are the
specialised versions of this keyword for small strain behaviours,
finite strain behaviours and cohesive zone models respectively. The
`@DrivingVariable` keyword is a synonym of `@Gradient`.

## Example

~~~~ {.cpp}
@Gradient {0.13};
~~~~~~~~
