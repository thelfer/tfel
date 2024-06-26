The `@Data` keyword allow the definition of a material properties using
the interpolation of a set of values.

The `@Data` keyword is followed by a set of options defining:

- the interpolation option,
- the set of values,
- the extrapolation option.

The precise syntax depends on the number of inputs of the material
property.

## Material properties without input

The only option accepted is `value`.

## Material properties with only one input

The `values` option is required. It must be a map associating values of
the input and values of the output.

The `interpolation` option is optional. It must be a string. The values
`linear` and `cubic_spline` are accepted.

The `extrapolation` option is optional. It must be a boolean or a
string:

- The boolean value `true` means that a linear extrapolation will be performed.
- The boolean value `false` or the string value `bound_to_last_value` or
  the string value `constant` means that a constant value will be used
  for extrapolation.

## Example of usage

~~~~{.cpp}
@DSL MaterialProperty;
@Law LinearDataInterpolation;

@UseQt true;
@UnitSystem SI;

@Output stress E;
E.setGlossaryName("YoungModulus");

@StateVariable temperature T;
T.setGlossaryName("Temperature");

@Data {
  values: { 293.15 : 240e9, 693.15 : 180e9, 893.15 : 170e9 },
  interpolation : "linear"
}
~~~~
