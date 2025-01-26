The `@TFELLibraries` keyword let the user specify TFEL libraries to link
with. This keyword must be followed by an array of strings.

The following libraries are available: `Config`, `Exception`,
`Glossary`, `Tests`, `UnicodeSupport`, `Utilities`, `System`, `Math`,
`MathCubicSpline`, `MathKriging`, `MathParser`, `NUMODIS`, `Material`,
`MFront`, `MTest`.

## Example

~~~~{.cpp}
@TFELLibraries {"MathParser"};
~~~~

