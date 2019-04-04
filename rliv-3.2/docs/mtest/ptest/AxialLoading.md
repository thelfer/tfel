The `AxialLoading` keyword let the user choose the kind of loading to
be applied to the pipe in the axial direction.

This keyword is followed by a string. The following values are
allowed:

- `None`: no loading is applied to the pipe.
- `EndCapEffect`: loading applied to the pipe model the inner and
  outer pressure effects on a closed pipe. See the `@RadialLoading`
  keyword to specify of those pressure are specified or computed. This
  is the default.
- `ImposedAxialForce`: the axial force acting of the pipe is specified
  by the user. See the `@AxialForceEvolution` for details.
- `ImposedAxialGrowth`: the axial growth of the pipe is specified by
  the user. See the `@AxialGrowthEvolution` for details.

## Example

~~~~ {.cpp}
@AxialLoading 'None';
~~~~~~~~
