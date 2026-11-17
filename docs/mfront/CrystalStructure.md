The crystal structure must be defined through the `@CrystalStructure`
keyword. This keyword is followed by one of the following value:

- `Cubic`: cubic structure.
- `BCC`: body centered cubic structure.
- `FCC`: face centered cubic structure.
- `HCP`: hexagonal closed-packed structures.

For the `HCP` structure, the `c_over_a_ratio` option gives the c/a
ratio, whose default value is the one of the ideal structure,
\(\sqrt{8/3}\).

## Examples

~~~~{.cpp}
@CrystalStructure FCC;
~~~~

~~~~{.cpp}
@CrystalStructure HCP{c_over_a_ratio : 1.593};
~~~~
