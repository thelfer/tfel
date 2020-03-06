The `@TangentOperatorBlock` keyword allows defining a the tangent
operator defined by a unique block.

This keyword disables the automatic declaration of the default tangent
operator block,s namely, the derivatives of all the fluxes with respect
to all the gradients.

Other tangent operator blocks still can be added using the
`@AdditionalTangentOperatorBlock` `@AdditionalTangentOperatorBlocks`
keywords, although we recommend using the `@TangentOperatorBlocks` to
define all the tangent operator blocks at once (and disable the default
tangent operator blocks).

The tangent operator blocks can be:

- the derivative of a flux with respect to a gradient.
- the derivative of a flux with respect to an external state variable.
- the derivative of a state variable with respect to an external state
  variable.

## Example

~~~~{.cpp}
@Gradient StrainStensor e₁;
e₁.setEntryName("MatrixStrain");
@Flux StressStensor σ₁;
σ₁.setEntryName("MatrixStress");

@Gradient StrainStensor e₂;
e₂.setEntryName("InclusionStrain");
@Flux StressStensor σ₂;
σ₂.setEntryName("InclusionStress");

@TangentOperatorBlock ∂σ₁∕∂Δe₁;
~~~~
