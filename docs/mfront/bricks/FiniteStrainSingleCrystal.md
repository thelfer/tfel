The `FiniteStrainSingleCrystal` brick hides the details of the
implementation of finite strain single crystal behaviours based on a
multiplicative decomposition of the deformation gradient.

## Options

The following boolean options are supported:

- `shifted_elastic_deformation_gradient`: if `true`, the auxiliary state
  variable `Fe` stores the elastic part of the deformation gradient minus
  the identity. This option is `false` by default.
- `plastic_deformation_gradient_normalisation_corrections`: if `true`,
  the jacobian and the consistent tangent operator take into account the
  normalisation of the inverse of the increment of the plastic part of
  the deformation gradient. If `false`, the derivatives of this
  normalisation are neglected. This option is `true` by default.

## Example

~~~~{.cpp}
@Brick FiniteStrainSingleCrystal{
  plastic_deformation_gradient_normalisation_corrections : false
};
~~~~
