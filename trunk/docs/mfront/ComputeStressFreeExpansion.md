The `ComputeStressFreeExansion` keyword introduces a code block which
allows the user to define an arbitrary expansion.

The code block is meant to **update** two symmetric tensors `dl0_l0`
and `dl1_l0` which respectively stands for the value of the expansion
at the begin and at the end of the time step.

## Note

- How `dl0_l0` and `dl1_l0` are treated depends on the behaviour type
  (small or finite strain, CZM) and on the finite strain strategies
  used, if any (in this case, the user have to refer the interface
  documentation).
- Local variables can't be used in a `@ComputeStressFreeExpansion`
  block, because stress free expansion are computed before the
  initialisation of those variables.

## Example

~~~~ {#ComputeStressFreeExpansion .cpp}
@ExternalStateVariable real tau;
tau.setGlossaryName("BurnUp (at.%)");

@Parameter Ctau = 0.6e-2;
Ctau.setEntryName("SolidSwellingCoefficient");

@ComputeStressFreeExpansion{
  dl0_l0 += Ctau*tau*StrainStensor::Id();
  dl1_l0 += Ctau*(tau+dtau)*StrainStensor::Id();
}
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


