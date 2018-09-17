% The generic behaviour interface
% Thomas Helfer
% 12/09/2018

The generic behaviour interface has been introduced to provide an
interface suitable for most needs. Whereas other interfaces target a
specific solver and thus are restricted by choices made by this specific
solver, this interface has been created for developers of homebrew
solvers who are able to modify their code to take full advantage of
`MFront` behaviours.

This interface is tightly linked with the
`MFrontGenericInterfaceSupport` project which is available on `github`:
<https://github.com/thelfer/MFrontGenericInterfaceSupport>. This project
has a more liberal licence which allows it to be included in both
commercial and open-source solvers/library. This licensing choice
explains why this project is not part of the `TFEL`.

# Features

## Supported behaviours

The generic behaviour interface supports:

- strain based behaviours. The definition of the strain depends of the
  strain measure used (see the `MFront`'`@StrainMeasure` keyword).
- finite strain behaviours.
- cohesive zone models

## Supported modelling hypotheses

- Axisymmetrical generalised plane strain.
        AXISYMMETRICAL,
        PLANESTRESS,
        PLANESTRAIN,
        GENERALISEDPLANESTRAIN,
        TRIDIMENSIONAL,

