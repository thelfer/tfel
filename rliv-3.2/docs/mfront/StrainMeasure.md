This keyword let the user choose to the strain measure. This choice
also defines the meaning of the stress tensor by energetic duality.

The following values are supported:

- `LINEARISED` (or `LINEARISED`, or `HPP`): the behaviour is meant to
  be used in a small strain analysis.
- `GreenLagrange`: the behaviour is meant to be used in a finite
  strain analysis. The dual stress is the second Piola-Kirchhoff
  stress.
- `Hencky`: the behaviour is meant to be used in a finite
  strain analysis. The dual stress has now specific name.

## Note

The computation of the strain measure and its dual and the convertion
of the tangent operator is delegated to the interface, because several
finite element solver already provides those operations natively
(`Code_Aster`, `ZebuloN`, etc...)

## Example

~~~~{#StrainMeasure .cpp}
@StrainMeasure Hencky;
~~~~
