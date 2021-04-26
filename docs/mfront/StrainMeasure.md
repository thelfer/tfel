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

Two options can be specified:

- `save_strain`: save the strain in a dedicated auxiliary state variable
  called `DualStrain`. Internally, the name of this auxiliary state
  variable is `saved_strain_measure`.
- `save_stress`: save the stress in a dedicated auxiliary state variable
  called `DualStress`. Internally, the name of this auxiliary state
  variable is `saved_dual_stress`.

If requested, the auxiliary state variables `saved_strain_measure` and
`saved_dual_stress` are updated at the beginning of the
`@UpdateAuxiliaryStateVariables` code block. For orthotropic behaviours,
it is worth mentionning that those variables are expressed in the
material frame.

## Note

The computation of the strain measure and its dual and the convertion
of the tangent operator is delegated to the interface, because several
finite element solver already provides those operations natively
(`Code_Aster`, `ZebuloN`, etc...)

## Example

~~~~{#StrainMeasure .cpp}
@StrainMeasure Hencky;
~~~~

~~~~{#StrainMeasure2 .cpp}
@StrainMeasure{
  save_strain: true,
  save_stress: true
};
~~~~
