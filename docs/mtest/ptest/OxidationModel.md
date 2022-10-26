The `@OxidationModel` keyword introduces a model which computes an
oxidation length at either the inner boundary or the outer boundary of
the pipe. This keyword must be followed by a data map with the following
entries:

- `model`: the name of model.
- `library`: the name of the library in which the model is available.
- `boundary`: the name of boundary on which the model is defined. This
  variable must be equal to `inner_boundary` or `outer_boundary`.

An oxidation model must define an internal state variable named
`OxidationLength`. The values of the material properties and external
state variables passed to an oxidation model are computed on the
boundary on which the model is defined.

The definition of a least one oxidation model automatically defines an
evolution named `OxidationStatus` which states if an integration point
is inside an oxidation layer.

## Example

~~~~{.cxx}
@OxidationModel{
  model : 'umatm5deziroxoxidationmodel_srma2020b',
  library : 'src/libUmatM5.so',
  boundary : 'outer_boundary'
};
~~~~