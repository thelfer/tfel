The `@OrthotropicBehaviour` declares the behaviour to be orthotropic.

As an option, the orthotropic behaviour can be followed by an
orthotropic axes convention. Three orthotropic axes convention are
currently supported:

- `Default`: no specific treatment. This can lead to serious
  difficulties as there is no way of ensuring the consistency of the
  definition of the orthotropic axes in most solvers.
- `Pipe`: the behaviour has been written using the Pipe convention
  described in the MFront behaviour manual. With this convention, the
  results of various keywords depends on the modelling hypothesis. For
  example, this option allows a consistent definition of the elastic
  material properties (see `@StiffnessTensor`, `@ElasticProperties`),
  thermal expansion coefficients (see `@ComputeThermalExpansion`) and
  the proper application of swelling (see `@AxialGrowth`, `Swelling`).
- `Plate`: when modelling plates, an appropriate convention is the following:
    - The first material axis is the rolling direction
    - The second material axis is the in plane direction perpendicular to
      the rolling direction (transverse direction).
    - The third material axis is the normal to the plate.
  By definition, this convention is only valid for \(3D\), \(2D\) plane
  stress, strain and generalized plane strain modelling hypotheses.

## Examples

~~~~{.cpp}
@OrthotropicBehaviour;
~~~~

~~~~{.cpp}
@OrthotropicBehaviour<Pipe>;
~~~~
