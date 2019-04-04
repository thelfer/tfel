The `@OrthotropicBehaviour` declares the behaviour to be orthotropic.

As an option, the orthotropic behaviour can be followed by an
orthotropic axes convention. Two orthotropic axes convention are
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

## Examples

~~~~{.cpp}
@OrthotropicBehaviour;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

~~~~{.cpp}
@OrthotropicBehaviour<Pipe>;
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
