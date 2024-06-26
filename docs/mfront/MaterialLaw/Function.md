The `@Function` keyword introduces the C++ code computing the value a
the material property.

Note: the default output is called `res`. This is changed by the
`@Output` keyword.

## Note

Although most interfaces use double for floatting point operations,
this may not be always the case. A special `typedef` named `real` is
thus introduced by the interfaces. We advice the user to use this
`typedef` to define their local variables.

## Example

~~~~{.cpp}
@Function
{
  const real C0 =    575.57;
  const real C1 = -21094.;
  Cp = C0 + C1 / T;
}
~~~~
