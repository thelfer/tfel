The `@Profile` keyword allows the user to create an new output file
containing the profile of some values defined at integration points such
as the components of the internal states variables, the linearised
string and the first Piola-Kirchhoff stress.

This keyword is followed by the name of the ouput file and an array
containing the names of the values to be post-processed.

6 components' names are defined by default:

- `ERR`, `ETT` and `EZZ` which denotes the components of the linearised
  strain
- `SRR`, `STT` and `SZZ` which denotes the components of the first
  Piola-Kirchhoff stress.

## Example

~~~~{.cxx}
@Profile 'elasticity-linear-profile.res' {'SRR','STT','SZZ'};
~~~~