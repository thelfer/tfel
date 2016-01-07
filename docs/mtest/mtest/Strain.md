The `@Strain` let the user specifiy the inital value of the strains.

This keyword is followed by an array. The size of this array must be
equal to the number of components of symmetric tensors for the
modelling hypothesis considered (see the `@ModellingHypothesis`
keyword).

## Note

The values must follow the `TFEL` conventions. In \(3D\), strains are
stored in the following order:
\[
\left(
varepsilon_{xx},\varepsilon_{yy},\varepsilon_{zz},\sqrt{2}\,\varepsilon_{xy},\sqrt{2}\,\varepsilon_{xz},\sqrt{2}\,\varepsilon_{yz}
\right)
\]

## Example

~~~~ {.cpp}
@Strain {0.000239466253465591,
         -7.18398760396772e-05,
         -7.18398760396772e-05,
         0.,0.,0.};
~~~~~~~~~~~
