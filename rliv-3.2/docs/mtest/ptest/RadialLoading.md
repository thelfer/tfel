The `RadialLoading` keyword let the user choose the kind of loading to
be applied to the pipe in radial direction.

This keyword is followed by a string. The following values are
allowed:

- `ImposedPressure`: the inner pressure is specified by the user. See
  the `@InnerPressureEvolution` for details. This is default.
- `TightPipe`: the total amount of substance of the gas is
  constant. The gas is perfect which allows:
	- the computation of the amount of substance using the filling
	  pressure and temperature. See the `@FillingPressure` and the
	  `@FillingTemperature` keywords for details.	
	- the computation of the inner pressure as a function of the
      curent temperature and enclosed volume.
- `ImposedOuterRadius`: the evolution of the outer radius is specified
  by the user. See the `@OuterRadiusEvolution` keyword for details.

## Example

~~~~ {.cpp}
@RadialLoading 'TightPipe';
~~~~~~~~
